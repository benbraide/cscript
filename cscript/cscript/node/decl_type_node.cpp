#include "decl_type_node.h"
#include "../common/env.h"

cscript::node::decl_type::decl_type(const lexer::token::index &index, ptr_type value)
	: basic(id::decl_type, index), value_(value){}

cscript::node::decl_type::~decl_type(){}

cscript::node::generic::ptr_type cscript::node::decl_type::clone(){
	return std::make_shared<decl_type>(index_, value_->clone());
}

bool cscript::node::decl_type::is(id id) const{
	return (id == node::id::decl_type || id == node::id::type_compatible);
}

cscript::object::generic *cscript::node::decl_type::evaluate(){
	auto value = get_type();
	return nullptr;
}

cscript::storage::generic *cscript::node::decl_type::get_storage(){
	auto value = get_type();
	return (value == nullptr) ? nullptr : value->query<storage::generic>();
}

cscript::type::generic::ptr_type cscript::node::decl_type::get_type(){
	auto value = value_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (value == nullptr)
		return common::env::error.set("", index_);

	return value->get_memory().type;
}

cscript::node::generic::ptr_type cscript::node::decl_type::get_value() const{
	return value_;
}

std::string cscript::node::decl_type::print_() const{
	return ("decltype(" + value_->print() + ")");
}
