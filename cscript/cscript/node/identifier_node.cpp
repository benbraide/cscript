#include "identifier_node.h"

#include "../common/env.h"
#include "../object/type_object.h"

cscript::node::identifier::identifier(const lexer::token::index &index, const std::string &value)
	: basic(id::identifier, index), value_(value){}

cscript::node::identifier::~identifier(){}

cscript::node::generic::ptr_type cscript::node::identifier::clone(){
	return std::make_shared<identifier>(index_, value_);
}

bool cscript::node::identifier::is(id id) const{
	return (id == node::id::identifier || id == node::id::identifier_compatible || id == node::id::type_compatible);
}

cscript::object::generic *cscript::node::identifier::evaluate(){
	storage::generic_value *value = nullptr;
	for (auto storage = common::env::runtime.storage; storage != nullptr; storage = storage->get_parent()){
		if ((value = storage->find(value_)) != nullptr)
			break;
	}

	if (value == nullptr)
		return common::env::error.set("", index_);

	if (value->is_storage()){

	}

	auto type_value = value->get_type();
	if (type_value != nullptr)
		return common::env::temp_storage.add(std::make_shared<object::primitive::type_object>(type_value));

	return value->get_object();
}

std::string cscript::node::identifier::get_key(){
	return value_;
}

const std::string &cscript::node::identifier::get_value() const{
	return value_;
}

std::string cscript::node::identifier::print_() const{
	return value_;
}
