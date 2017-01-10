#include "return_node.h"
#include "../common/env.h"

cscript::node::return_node::return_node(const lexer::token::index &index, ptr_type value)
	: basic(id::return_, index), value_(value){}

cscript::node::return_node::~return_node(){}

cscript::node::generic::ptr_type cscript::node::return_node::clone(){
	if (value_ == nullptr)
		return std::make_shared<return_node>(index_, nullptr);

	return std::make_shared<return_node>(index_, value_->clone());
}

cscript::object::generic *cscript::node::return_node::evaluate(){
	if (value_ == nullptr)
		return common::env::error.set(common::error::type::return_);

	auto value = value_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (value == nullptr)
		return common::env::error.set("void value in expression", index_);

	return common::env::error.set(common::error::type::return_, value->get_ptr());
}

cscript::node::generic::ptr_type cscript::node::return_node::get_value() const{
	return value_;
}

std::string cscript::node::return_node::print_() const{
	if (value_ == nullptr)
		return "return";

	return ("return " + value_->print());
}
