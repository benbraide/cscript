#include "binary_operator_node.h"
#include "../common/env.h"

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
	auto left = left_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (left == nullptr)
		return common::env::error.set("", left_->get_index());

	common::env::object_operand = nullptr;
	common::env::node_operand = right_.get();

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
