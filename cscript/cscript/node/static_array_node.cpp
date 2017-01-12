#include "static_array_node.h"

#include "../type/primitive_type.h"
#include "../common/env.h"
#include "../object/static_array_object.h"

cscript::node::static_array::static_array(const lexer::token::index &index, ptr_type declaration, ptr_type size)
	: basic(id::declaration, index), declaration_(declaration), size_(size){}

cscript::node::static_array::~static_array(){}

cscript::node::generic::ptr_type cscript::node::static_array::clone(){
	return std::make_shared<static_array>(index_, declaration_->clone(), size_->clone());
}

cscript::object::generic *cscript::node::static_array::evaluate(){
	if (get_type_value() == nullptr)
		return common::env::error.set("Type not found", index_);

	auto key = get_identifier()->get_key();
	if (key.empty())
		return common::env::error.set("Invalid key", index_);

	if (common::env::runtime.storage->find(key) != nullptr)
		return common::env::error.set("'" + get_identifier()->print() + "' name already exists", index_);

	auto object = type_value_->create(type_value_);
	if (object == nullptr)
		return common::env::error.set("Declaration failure", index_);

	common::env::runtime.declaration.value = &common::env::runtime.storage->add(key);
	common::env::runtime.declaration.value->set(object);

	auto &memory_entry = object->get_memory();
	memory_entry.info.storage = common::env::runtime.storage;
	CSCRIPT_SET(memory_entry.attributes, get_type_attributes());

	return object.get();
}

cscript::node::generic::ptr_type cscript::node::static_array::get_declaration() const{
	return declaration_;
}

cscript::node::generic::ptr_type cscript::node::static_array::get_size() const{
	return size_;
}

cscript::node::generic::ptr_type cscript::node::static_array::get_type() const{
	return declaration_->query<declaration>()->get_type();
}

cscript::node::generic::ptr_type cscript::node::static_array::get_identifier() const{
	return declaration_->query<declaration>()->get_identifier();
}

cscript::memory::virtual_address::attribute cscript::node::static_array::get_type_attributes() const{
	return declaration_->query<declaration>()->get_type_attributes();
}

cscript::type::generic::ptr_type cscript::node::static_array::get_type_value(){
	if (type_value_ == nullptr){
		auto base_type = declaration_->query<declaration>()->get_type_value();
		if (base_type == nullptr)
			return nullptr;

		auto size = size_->evaluate();
		if (common::env::error.has() || size == nullptr)
			return nullptr;

		auto numeric_size = size->query<object::primitive::numeric>();
		if (numeric_size == nullptr || !numeric_size->get_type()->is_integral())
			return nullptr;

		auto size_value = numeric_size->get_value<int>();
		if (size_value <= 0)
			return nullptr;

		type_value_ = std::make_shared<type::static_array>(base_type, size_value);
	}

	return type_value_;
}

bool cscript::node::static_array::is_static() const{
	return declaration_->query<declaration>()->is_static();
}

std::string cscript::node::static_array::print_() const{
	return (declaration_->print() + "[" + size_->print() + "]");
}
