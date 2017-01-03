#include "typedef_node.h"
#include "../common/env.h"

cscript::node::type_definition::type_definition(const lexer::token::index &index, ptr_type type, ptr_type name)
	: basic(id::typedef_, index), type_(type), name_(name){}

cscript::node::type_definition::~type_definition(){}

cscript::node::generic::ptr_type cscript::node::type_definition::clone(){
	return std::make_shared<type_definition>(index_, type_->clone(), name_->clone());
}

cscript::object::generic *cscript::node::type_definition::evaluate(){
	auto type = type_->get_type();
	if (type == nullptr)
		return common::env::error.set("", index_);

	std::string name;
	if (name_->is(id::placeholder)){
		auto value = name_->evaluate();
		if (common::env::error.has())
			return nullptr;

		if (value == nullptr)
			return common::env::error.set("", index_);

		name = value->to_string();
		if (common::env::error.has())
			return nullptr;
	}
	else//Identifier
		name = name_->print();

	if (common::env::runtime.storage->find(name) != nullptr)
		return common::env::error.set("", index_);

	common::env::runtime.declaration.value = &common::env::runtime.storage->add(name);
	common::env::runtime.declaration.value->set(type);

	return nullptr;
}

cscript::node::generic::ptr_type cscript::node::type_definition::get_type() const{
	return type_;
}

cscript::node::generic::ptr_type cscript::node::type_definition::get_name() const{
	return name_;
}

std::string cscript::node::type_definition::print_() const{
	return ("typedef " + type_->print() + " " + name_->print());
}
