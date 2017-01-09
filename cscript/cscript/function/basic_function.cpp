#include "basic_function.h"

#include "../common/env.h"
#include "../node/collection_node.h"

cscript::function::basic::basic(const std::string &name, storage::generic *storage)
	: name_(name), storage_(storage){}

cscript::function::basic::~basic(){}

cscript::object::generic *cscript::function::basic::call(){
	for (auto argument : common::env::runtime.operand.node->query<node::collection>()->get_list()){
		common::env::runtime.arguments.push_back(argument->evaluate());
		if (common::env::error.has()){
			common::env::runtime.arguments.clear();
			return nullptr;
		}
	}

	auto score = 0;
	auto matched = get_matched(score);
	if (matched == nullptr){
		common::env::runtime.arguments.clear();
		return common::env::error.set("");
	}

	auto definition = matched->get_definition();
	if (definition == nullptr){
		common::env::runtime.arguments.clear();
		return common::env::error.set("");
	}

	return definition->call(matched->get_storage());
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

cscript::storage::generic *cscript::function::basic::get_storage(){
	return storage_;
}

std::string cscript::function::basic::print(){
	shared_lock_type guard(lock_);
	return (list_.size() == 1u) ? (*list_.begin())->print() : "";
}

const std::string &cscript::function::basic::get_name() const{
	return name_;
}

void cscript::function::basic::set_definition(definition &definition){}
