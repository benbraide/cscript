#include "echo_node.h"
#include "../common/env.h"

cscript::node::echo::echo(const lexer::token::index &index, ptr_type value)
	: basic(id::echo, index), value_(value){}

cscript::node::echo::~echo(){}

cscript::node::generic::ptr_type cscript::node::echo::clone(){
	return std::make_shared<echo>(index_, value_->clone());
}

cscript::object::generic *cscript::node::echo::evaluate(){
	auto value = value_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (value == nullptr)
		return common::env::error.set("void value in expression", index_);

	auto string_value = value->echo();
	if (common::env::error.has())
		return nullptr;

	common::env::echo(string_value);
	return nullptr;
}

cscript::node::generic::ptr_type cscript::node::echo::get_value() const{
	return value_;
}

std::string cscript::node::echo::print_() const{
	return ("echo " + value_->print());
}
