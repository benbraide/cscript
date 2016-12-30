#include "basic_node.h"
#include "../common/env.h"

cscript::node::basic::basic(id id, const lexer::token::index &index)
	: id_(id), index_(index){}

cscript::node::basic::~basic(){}

cscript::node::id cscript::node::basic::get_id() const{
	return id_;
}

const cscript::lexer::token::index &cscript::node::basic::get_index() const{
	return index_;
}

bool cscript::node::basic::is(id id) const{
	return (id == id_);
}

std::string cscript::node::basic::print() const{
	return print_();
}

std::string cscript::node::basic::get_key(){
	return "";
}

cscript::storage::generic *cscript::node::basic::get_storage(){
	auto key = get_key();
	return key.empty() ? nullptr : common::env::runtime.storage->get_matched(key);
}

cscript::type::generic::ptr_type cscript::node::basic::get_type(){
	auto key = get_key();
	if (key.empty())
		return nullptr;

	type::generic::ptr_type value;
	storage::generic_value *found_value;

	for (auto storage = common::env::runtime.storage; storage != nullptr; storage = storage->get_parent()){
		if ((found_value = storage->find(key)) != nullptr){
			value = found_value->get_type();
			break;
		}
	}

	return value;
}

std::string cscript::node::basic::print_() const{
	return "";
}
