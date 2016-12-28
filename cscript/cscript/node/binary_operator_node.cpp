#include "binary_operator_node.h"
#include "../common/env.h"

cscript::node::binary_operator::binary_operator(const lexer::token::index &index, const info_type &info, ptr_type left, ptr_type right, generic *parent)
	: basic(id::operator_, index, parent), info_(info), left_(left), right_(right){}

cscript::node::binary_operator::~binary_operator(){}

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
