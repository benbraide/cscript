#include "function_entry.h"

#include "../common/env.h"
#include "../type/primitive_type.h"

cscript::function::entry::entry(const std::string &name, storage::generic *storage,
	type::generic::ptr_type type, const parameter_limits &parameter_limits)
	: basic(name, storage), definition_(nullptr), type_(type), parameter_limits_(parameter_limits){}

cscript::function::entry::~entry(){}

cscript::function::generic *cscript::function::entry::get_matched(int &score){
	auto argument_count = static_cast<int>(common::env::runtime.arguments.size());
	if (argument_count < parameter_limits_.minimum ||
		(parameter_limits_.maximum != -1 && parameter_limits_.maximum < argument_count)){
		return basic::get_matched(score);
	}

	if (argument_count == 0)
		return this;

	auto minimum_score = 0, current_score = 0;
	auto type_iter = type_->query<type::function>()->get_parameter_types().begin();

	for (auto argument : common::env::runtime.arguments){
		if ((current_score = (*type_iter)->get_score(argument->get_type().get())) == 0)
			break;

		if (minimum_score == 0 || current_score < minimum_score)
			minimum_score = current_score;

		if (!(*type_iter)->is_variadic())
			++type_iter;
	}

	if (minimum_score < score)
		return basic::get_matched(score);

	score = minimum_score;
	return this;
}

cscript::function::generic *cscript::function::entry::get_matched(const type::generic *type){
	return type_->is_same(type) ? this : basic::get_matched(type);
}

cscript::type::generic::ptr_type cscript::function::entry::get_type(){
	return type_;
}

cscript::function::definition *cscript::function::entry::get_definition(){
	return definition_.get();
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

void cscript::function::entry::set_definition(definition_type definition){
	definition_ = definition;
}
