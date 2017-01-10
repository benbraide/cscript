#include "binary_operator_node.h"

#include "../common/env.h"
#include "../object/type_object.h"
#include "../type/choice_type.h"

cscript::node::binary_operator::binary_operator(const lexer::token::index &index, const info_type &info, ptr_type left, ptr_type right)
	: basic(id::operator_, index), info_(info), left_(left), right_(right){}

cscript::node::binary_operator::~binary_operator(){}

cscript::node::generic::ptr_type cscript::node::binary_operator::clone(){
	return std::make_shared<binary_operator>(index_, info_, left_->clone(), right_->clone());
}

bool cscript::node::binary_operator::is(id id) const{
	if (id == node::id::operator_)
		return true;

	if (info_.id == lexer::operator_id::bitwise_or){
		if (id == node::id::type_compatible)
			return (left_->is(id) && right_->is(id));

		return (id == node::id::auto_type || id == node::id::variadic_type || id == node::id::array_type || id == node::id::function_type ||
			id == node::id::pointer_type) ? (is(node::id::type_compatible) && (left_->is(id) || right_->is(id))) : false;
	}

	if (id == node::id::identifier_compatible || id == node::id::type_compatible)
		return (info_.id == lexer::operator_id::scope_resolution && left_->is(id) && right_->is(id));

	return false;
}

cscript::object::generic *cscript::node::binary_operator::evaluate(){
	if (info_.id == lexer::operator_id::scope_resolution){
		auto storage = left_->get_storage();
		if (storage == nullptr)
			return common::env::error.set("'" + left_->print() + "' namespace not found", index_);

		auto value = storage->find(right_->get_key());
		if (value == nullptr)
			return common::env::error.set("'" + print() + "' name not found", index_);

		auto type_value = value->get_type();
		if (type_value != nullptr)
			return common::env::temp_storage.add(std::make_shared<object::primitive::type_object>(type_value));

		return value->get_object();
	}

	auto left = left_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (left == nullptr)
		return common::env::error.set("void value in expression", left_->get_index());

	switch (info_.id){
	case lexer::operator_id::relational_or:
		return short_circuit_(left, true, type::boolean_value_type::true_);
	case lexer::operator_id::relational_and:
		return short_circuit_(left, false, type::boolean_value_type::false_);
	case lexer::operator_id::explicit_equality:
		return explicit_compare_(left, true);
	case lexer::operator_id::explicit_inverse_equality:
		return explicit_compare_(left, false);
	default:
		break;
	}

	common::env::runtime.operand = { right_.get() };
	return left->evaluate(info_);
}

cscript::storage::generic *cscript::node::binary_operator::get_storage(){
	if (info_.id == lexer::operator_id::scope_resolution){
		auto storage = left_->get_storage();
		if (storage == nullptr)
			return nullptr;

		auto value = storage->find(right_->get_key());
		return (value == nullptr) ? nullptr : value->get_storage();
	}

	return nullptr;
}

cscript::type::generic::ptr_type cscript::node::binary_operator::get_type(){
	if (info_.id == lexer::operator_id::scope_resolution){
		auto storage = left_->get_storage();
		if (storage == nullptr)
			return nullptr;

		auto value = storage->find(right_->get_key());
		return (value == nullptr) ? nullptr : value->get_type();
	}

	if (info_.id == lexer::operator_id::bitwise_or){
		auto left = left_->get_type();
		if (left == nullptr)
			return nullptr;

		auto right = right_->get_type();
		if (right == nullptr)
			return nullptr;

		return std::make_shared<type::choice>(left, right);
	}

	return nullptr;
}

const cscript::node::binary_operator::info_type &cscript::node::binary_operator::get_info() const{
	return info_;
}

cscript::node::generic::ptr_type cscript::node::binary_operator::get_left() const{
	return left_;
}

cscript::node::generic::ptr_type cscript::node::binary_operator::get_right() const{
	return right_;
}

cscript::object::generic *cscript::node::binary_operator::short_circuit_(object::generic *left, bool truth, type::boolean_value_type value){
	auto boolean_value = left->to_bool();
	if (common::env::error.has())
		return nullptr;

	if (boolean_value == truth)
		return common::env::temp_storage.add(std::make_shared<object::primitive::boolean>(value));

	auto right = right_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (right == nullptr)
		return common::env::error.set("void value in expression", left_->get_index());

	boolean_value = right->to_bool();
	if (common::env::error.has())
		return nullptr;

	return common::env::temp_storage.add(std::make_shared<object::primitive::boolean>(boolean_value ?
		type::boolean_value_type::true_ : type::boolean_value_type::false_));
}

cscript::object::generic *cscript::node::binary_operator::explicit_compare_(object::generic *left, bool truth){
	auto right = right_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (right == nullptr)
		return common::env::error.set("void value in expression", left_->get_index());

	if (!left->get_type()->is_same(right->get_type().get())){
		return common::env::temp_storage.add(std::make_shared<object::primitive::boolean>(truth ?
			type::boolean_value_type::false_ : type::boolean_value_type::true_));
	}

	common::env::runtime.operand = { nullptr, right };
	return left->evaluate(info_type{ truth ? lexer::operator_id::equality : lexer::operator_id::inverse_equality });
}

std::string cscript::node::binary_operator::print_() const{
	return (left_->print() + print_open_() + right_->print() + print_close_());
}

std::string cscript::node::binary_operator::print_open_() const{
	switch (info_.id){
	case lexer::operator_id::member_access:
	case lexer::operator_id::member_pointer_access:
	case lexer::operator_id::scope_resolution:
	case lexer::operator_id::ellipsis:
	case lexer::operator_id::call:
	case lexer::operator_id::index:
		return info_.value;
	case lexer::operator_id::comma:
		return (info_.value + " ");
	default:
		break;
	}

	return (" " + info_.value + " ");
}

std::string cscript::node::binary_operator::print_close_() const{
	switch (info_.id){
	case lexer::operator_id::call:
		return ")";
	case lexer::operator_id::index:
		return "]";
	default:
		break;
	}

	return "";
}
