#include "declaration_node.h"
#include "type_node.h"
#include "../common/env.h"

cscript::node::declaration::declaration(const lexer::token::index &index, ptr_type type, ptr_type id)
	: basic(id::declaration, index), type_(type), id_(id){}

cscript::node::declaration::~declaration(){}

cscript::node::generic::ptr_type cscript::node::declaration::clone(){
	return std::make_shared<declaration>(index_, type_->clone(), (id_ == nullptr) ? nullptr : id_->clone());
}

cscript::object::generic *cscript::node::declaration::evaluate(){
	auto type = type_->get_type();
	if (type == nullptr)
		return common::env::error.set("", type_->get_index());

	auto object = type->create(type);
	if (object == nullptr)
		return common::env::error.set("", type_->get_index());

	common::env::runtime.declaration.object = object;
	if (id_ != nullptr){//Named
		auto key = id_->get_key();
		if (key.empty())
			return common::env::error.set("", type_->get_index());

		if (common::env::runtime.storage->find(key) != nullptr)
			return common::env::error.set("", type_->get_index());

		common::env::runtime.declaration.value = &common::env::runtime.storage->add(key);
		common::env::runtime.declaration.value->set(object);

		object->get_memory().storage = common::env::runtime.storage;
	}

	return object.get();
}

cscript::node::generic::ptr_type cscript::node::declaration::get_type() const{
	return type_;
}

cscript::node::generic::ptr_type cscript::node::declaration::get_identifier() const{
	return id_;
}

cscript::memory::virtual_address::attribute cscript::node::declaration::get_type_attributes() const{
	auto with_storage = type_->query<type_with_storage_class>();
	return (with_storage == nullptr) ? memory::virtual_address::attribute::nil : with_storage->get_attributes();
}

std::string cscript::node::declaration::print_() const{
	return (type_->print() + " " + ((id_ == nullptr) ? "" : id_->print()));
}
