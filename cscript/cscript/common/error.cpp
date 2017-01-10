#include "error.h"
#include "env.h"

std::nullptr_t cscript::common::error::set(object_type object){
	if (object_ == nullptr){
		type_ = type::nil;
		object_ = object;
	}
	return nullptr;
}

std::nullptr_t cscript::common::error::set(type type, object_type object){
	if (object_ == nullptr){
		type_ = type;
		object_ = object;
	}
	return nullptr;
}

std::nullptr_t cscript::common::error::set(const std::string &value){
	return set(value, env::runtime.index);
}

std::nullptr_t cscript::common::error::set(const std::string &value, const index_type &index){
	return set(env::create_string("<line: " + std::to_string(index.line) + ", column: " + std::to_string(index.column) + "> " + value));
}

void cscript::common::error::warn(const std::string &value){
	env::echo(value);
}

void cscript::common::error::clear(){
	type_ = type::nil;
	object_ = nullptr;
}

void cscript::common::error::report(){
	if (object_ == nullptr){
		switch (type_){
		case type::return_:
			env::echo("Error: Invalid 'return' statement.");
			type_ = type::nil;
			break;
		case type::break_:
			env::echo("Error: Invalid 'break' statement.");
			type_ = type::nil;
			break;
		case type::continue_:
			env::echo("Error: Invalid 'continue' statement.");
			type_ = type::nil;
			break;
		default:
			break;
		}

		return;
	}

	auto object = object_;
	clear();

	auto value = object->to_string();
	if (object_ != nullptr){
		clear();
		value = "<Object>";
	}

	env::echo("Error: " + value + ".");
}

cscript::common::error::object_type cscript::common::error::get(){
	return object_;
}

bool cscript::common::error::has() const{
	return (object_ != nullptr);
}

bool cscript::common::error::is_return() const{
	return (type_ == type::return_);
}

bool cscript::common::error::is_break() const{
	return (type_ == type::break_);
}

bool cscript::common::error::is_continue() const{
	return (type_ == type::continue_);
}
