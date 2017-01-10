#include "decl_type_node.h"

#include "../common/env.h"
#include "../object/type_object.h"
#include "../object/pointer_object.h"

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
	auto type_value = get_type();
	if (type_value == nullptr)
		return common::env::error.set("'" + value_->print() + "' type not found", index_);

	return common::env::temp_storage.add(std::make_shared<object::primitive::type_object>(type_value));
}

cscript::storage::generic *cscript::node::decl_type::get_storage(){
	auto value = get_type();
	return (value == nullptr) ? nullptr : value->query<storage::generic>();
}

cscript::type::generic::ptr_type cscript::node::decl_type::get_type(){
	auto value = value_->evaluate();
	return (common::env::error.has() || value == nullptr) ? nullptr : value->get_type();
}

cscript::node::generic::ptr_type cscript::node::decl_type::get_value() const{
	return value_;
}

std::string cscript::node::decl_type::print_() const{
	return ("decltype(" + value_->print() + ")");
}
