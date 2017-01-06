#include "declaration_node.h"
#include "type_node.h"

#include "../common/env.h"
#include "../object/ref_object.h"

cscript::node::declaration::declaration(const lexer::token::index &index, ptr_type type, ptr_type id)
	: basic(id::declaration, index), type_(type), id_(id){}

cscript::node::declaration::~declaration(){}

cscript::node::generic::ptr_type cscript::node::declaration::clone(){
	return std::make_shared<declaration>(index_, type_->clone(), (id_ == nullptr) ? nullptr : id_->clone());
}

cscript::object::generic *cscript::node::declaration::evaluate(){
	return allocate(index_, get_type_value(), id_, get_type_attributes());
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

cscript::type::generic::ptr_type cscript::node::declaration::get_type_value(){
	return (type_value_ == nullptr) ? (type_value_ = type_->get_type()) : type_value_;
}

cscript::object::generic *cscript::node::declaration::allocate(const lexer::token::index &index, type::generic::ptr_type type,
	ptr_type id, memory::virtual_address::attribute attributes){
	if (type == nullptr)
		return common::env::error.set("", index);

	object::generic::ptr_type object;
	if (CSCRIPT_IS(attributes, memory::virtual_address::attribute::ref))
		object = std::make_shared<cscript::object::ref>(type);
	else//Create uninitialized object
		object = type->create(type);

	if (object == nullptr)
		return common::env::error.set("", index);

	common::env::runtime.declaration.object = object;
	if (id != nullptr){//Named
		auto key = id->get_key();
		if (key.empty())
			return common::env::error.set("", index);

		if (common::env::runtime.storage->find(key) != nullptr)
			return common::env::error.set("", index);

		common::env::runtime.declaration.value = &common::env::runtime.storage->add(key);
		common::env::runtime.declaration.value->set(object);

		auto &memory_entry = object->get_memory();
		memory_entry.info.storage = common::env::runtime.storage;
		CSCRIPT_SET(memory_entry.attributes, attributes);
	}

	return object.get();
}

std::string cscript::node::declaration::print_() const{
	return (type_->print() + " " + ((id_ == nullptr) ? "" : id_->print()));
}

cscript::node::dependent_declaration::dependent_declaration(const lexer::token::index &index, ptr_type declaration, ptr_type id)
	: basic(id::declaration, index), declaration_(declaration), id_(id){}

cscript::node::dependent_declaration::~dependent_declaration(){}

cscript::node::generic::ptr_type cscript::node::dependent_declaration::clone(){
	return std::make_shared<dependent_declaration>(index_, declaration_->clone(), id_->clone());
}

cscript::object::generic *cscript::node::dependent_declaration::evaluate(){
	return declaration::allocate(declaration_->get_index(), get_type_value(), id_, get_type_attributes());
}

cscript::node::generic::ptr_type cscript::node::dependent_declaration::get_type() const{
	auto declaration_node = declaration_->query<declaration>();
	if (declaration_node != nullptr)
		return declaration_node->get_type();

	auto dependent_declaration_node = declaration_->query<dependent_declaration>();
	if (dependent_declaration_node != nullptr)
		return dependent_declaration_node->get_type();

	return declaration_->query<initialization_declaration>()->get_type();
}

cscript::node::generic::ptr_type cscript::node::dependent_declaration::get_identifier() const{
	return id_;
}

cscript::node::generic::ptr_type cscript::node::dependent_declaration::get_declaration() const{
	return declaration_;
}

cscript::memory::virtual_address::attribute cscript::node::dependent_declaration::get_type_attributes() const{
	auto declaration_node = declaration_->query<declaration>();
	if (declaration_node != nullptr)
		return declaration_node->get_type_attributes();

	auto dependent_declaration_node = declaration_->query<dependent_declaration>();
	if (dependent_declaration_node != nullptr)
		return dependent_declaration_node->get_type_attributes();

	return declaration_->query<initialization_declaration>()->get_type_attributes();
}

cscript::type::generic::ptr_type cscript::node::dependent_declaration::get_type_value(){
	auto declaration_node = declaration_->query<declaration>();
	if (declaration_node != nullptr)
		return declaration_node->get_type_value();

	auto dependent_declaration_node = declaration_->query<dependent_declaration>();
	if (dependent_declaration_node != nullptr)
		return dependent_declaration_node->get_type_value();

	return declaration_->query<initialization_declaration>()->get_type_value();
}

std::string cscript::node::dependent_declaration::print_() const{
	return (declaration_->print() + ", " + id_->print());
}

cscript::node::initialization_declaration::initialization_declaration(const lexer::token::index &index, ptr_type declaration, ptr_type value)
	: basic(id::declaration, index), declaration_(declaration), value_(value){}

cscript::node::initialization_declaration::~initialization_declaration(){}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::clone(){
	return std::make_shared<initialization_declaration>(index_, declaration_->clone(), value_->clone());
}

cscript::object::generic *cscript::node::initialization_declaration::evaluate(){
	auto value = value_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (value == nullptr)
		return common::env::error.set("", declaration_->get_index());

	auto *object = declaration_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (object == nullptr)
		return common::env::error.set("", declaration_->get_index());

	common::env::object_operand = value;
	return object->evaluate(object::generic::binary_info{ lexer::operator_id::assignment });
}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::get_type() const{
	auto declaration_node = declaration_->query<declaration>();
	if (declaration_node != nullptr)
		return declaration_node->get_type();

	return declaration_->query<dependent_declaration>()->get_type();
}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::get_identifier() const{
	auto declaration_node = declaration_->query<declaration>();
	if (declaration_node != nullptr)
		return declaration_node->get_identifier();

	return declaration_->query<dependent_declaration>()->get_identifier();
}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::get_declaration() const{
	return declaration_;
}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::get_value() const{
	return value_;
}

cscript::memory::virtual_address::attribute cscript::node::initialization_declaration::get_type_attributes() const{
	auto declaration_node = declaration_->query<declaration>();
	if (declaration_node != nullptr)
		return declaration_node->get_type_attributes();

	return declaration_->query<dependent_declaration>()->get_type_attributes();
}

cscript::type::generic::ptr_type cscript::node::initialization_declaration::get_type_value(){
	auto declaration_node = declaration_->query<declaration>();
	if (declaration_node != nullptr)
		return declaration_node->get_type_value();

	return declaration_->query<dependent_declaration>()->get_type_value();
}

std::string cscript::node::initialization_declaration::print_() const{
	return (declaration_->print() + " = " + value_->print());
}
