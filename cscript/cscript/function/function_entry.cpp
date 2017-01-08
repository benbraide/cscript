#include "function_entry.h"

#include "../common/env.h"
#include "../type/primitive_type.h"

cscript::function::entry::entry(std::string &name, type::generic::ptr_type type)
	: basic(name), definition_(nullptr), type_(type){}

cscript::function::entry::~entry(){}

cscript::function::generic *cscript::function::entry::get_matched(int &score){
	return basic::get_matched(score);
}

cscript::function::generic *cscript::function::entry::get_matched(const type::generic *type){
	return type_->is_same(type) ? this : basic::get_matched(type);
}

cscript::type::generic::ptr_type cscript::function::entry::get_type(){
	return type_;
}

cscript::function::definition *cscript::function::entry::get_definition(){
	return definition_;
}

std::string cscript::function::entry::print(){
	auto function_type = type_->query<type::function>();
	if (function_type == nullptr){
		common::env::error.set("");
		return "";
	}

	std::string parameter_types_string;
	auto &paramter_types = function_type->get_parameter_types();
	for (auto paramter_type : paramter_types){
		if (parameter_types_string.empty())
			parameter_types_string = paramter_type->print();
		else//Append
			parameter_types_string += (", " + paramter_type->print());
	}

	return (function_type->get_return_type()->print() + " " + get_name() + "(" + parameter_types_string + ")");
}

void cscript::function::entry::set_definition(definition &definition){
	definition_ = &definition;
}
