#include "placeholder_node.h"
#include "../common/env.h"

cscript::node::placeholder::placeholder(const lexer::token::index &index, ptr_type value)
	: basic(id::placeholder, index), value_(value){}

cscript::node::placeholder::~placeholder(){}

cscript::node::generic::ptr_type cscript::node::placeholder::clone(){
	return std::make_shared<placeholder>(index_, value_->clone());
}

bool cscript::node::placeholder::is(id id) const{
	return (id == node::id::placeholder || id == node::id::identifier_compatible || id == node::id::type_compatible);
}

cscript::object::generic *cscript::node::placeholder::evaluate(){
	auto key = get_key();
	if (common::env::error.has())
		return nullptr;

	if (key.empty())
		return common::env::error.set("", index_);

	storage::generic_value *value = nullptr;
	for (auto storage = common::env::runtime.storage; storage != nullptr; storage = storage->get_parent()){
		if ((value = storage->find(key)) != nullptr)
			break;
	}

	if (value == nullptr)
		return common::env::error.set("", index_);

	if (value->is_storage()){

	}

	if (value->is_type()){

	}

	return value->get_object();
}

std::string cscript::node::placeholder::get_key(){
	auto value = value_->evaluate();
	if (common::env::error.has())
		return "";

	if (value == nullptr){
		common::env::error.set("", index_);
		return "";
	}

	return value->to_string();
}

cscript::node::generic::ptr_type cscript::node::placeholder::get_value() const{
	return value_;
}

std::string cscript::node::placeholder::print_() const{
	return ("__placeholder(" + value_->print() + ")");
}
