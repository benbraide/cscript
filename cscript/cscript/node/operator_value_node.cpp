#include "operator_value_node.h"
#include "../common/env.h"

cscript::node::operator_value::operator_value(const lexer::token::index &index, const key &key)
	: basic(id::operator_value_, index), key_(key){}

cscript::node::operator_value::~operator_value(){}

cscript::node::generic::ptr_type cscript::node::operator_value::clone(){
	if (key_.type == nullptr)
		return std::make_shared<operator_value>(index_, key_);

	return std::make_shared<operator_value>(index_, key{ key_.right, key_.value, key_.type->clone() });
}

cscript::object::generic *cscript::node::operator_value::evaluate(){
	return nullptr;
}

std::string cscript::node::operator_value::get_key(){
	if (string_key_.empty()){
		string_key_ = (key_.right ? ".." : ".");
		if (key_.type != nullptr){
			auto type = key_.type->get_type();
			if (common::env::error.has() || type == nullptr)
				string_key_ += type->print();
			else//Error
				string_key_.clear();
		}
		else//Symbol
			string_key_ += key_.value;
	}

	return string_key_;
}

const cscript::node::operator_value::key &cscript::node::operator_value::get_key_value() const{
	return key_;
}

std::string cscript::node::operator_value::print_() const{
	if (key_.right)
		return ("operator \\right " + (key_.type == nullptr) ? key_.value : key_.type->print());

	return ("operator " + (key_.type == nullptr) ? key_.value : key_.type->print());
}
