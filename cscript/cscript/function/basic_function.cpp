#include "basic_function.h"
#include "../common/env.h"

cscript::function::basic::basic(std::string &name)
	: name_(&name){}

cscript::function::basic::~basic(){}

cscript::object::generic *cscript::function::basic::call(){
	auto score = 0;
	auto matched = get_matched(score);
	if (matched == nullptr)
		return common::env::error.set("");

	auto definition = matched->get_definition();
	if (definition == nullptr)
		return common::env::error.set("");

	return definition->call();
}

cscript::function::generic &cscript::function::basic::add(generic &value){
	guard_type guard(lock_);
	list_.push_back(&value);
	return *this;
}

cscript::function::generic *cscript::function::basic::get_matched(int &score){
	shared_lock_type guard(lock_);

	generic *matched = nullptr, *next = nullptr;
	for (auto item : list_){
		if ((next = item->get_matched(score)) != nullptr)
			matched = next;
	}

	return matched;
}

cscript::function::generic *cscript::function::basic::get_matched(const type::generic *type){
	shared_lock_type guard(lock_);

	generic *matched = nullptr;
	for (auto item : list_){
		if ((matched = item->get_matched(type)) != nullptr)
			break;
	}

	return matched;
}

cscript::type::generic::ptr_type cscript::function::basic::get_type(){
	shared_lock_type guard(lock_);
	return (list_.size() == 1u) ? (*list_.begin())->get_type() : nullptr;
}

cscript::function::definition *cscript::function::basic::get_definition(){
	shared_lock_type guard(lock_);
	return (list_.size() == 1u) ? (*list_.begin())->get_definition() : nullptr;
}

std::string cscript::function::basic::print(){
	shared_lock_type guard(lock_);
	return (list_.size() == 1u) ? (*list_.begin())->print() : "";
}

const std::string &cscript::function::basic::get_name() const{
	return *name_;
}

void cscript::function::basic::set_definition(definition &definition){}
