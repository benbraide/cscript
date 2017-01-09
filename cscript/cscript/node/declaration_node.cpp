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
	return allocate();
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

cscript::object::generic *cscript::node::declaration::allocate(){
	return allocate(index_, get_type_value(), get_identifier(), get_type_attributes());
}

cscript::object::generic *cscript::node::declaration::allocate(type::generic::ptr_type type){
	return allocate(index_, type, get_identifier(), get_type_attributes());
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
	: declaration(index, declaration, id){}

cscript::node::dependent_declaration::~dependent_declaration(){}

cscript::node::generic::ptr_type cscript::node::dependent_declaration::clone(){
	return std::make_shared<dependent_declaration>(index_, type_->clone(), id_->clone());
}

cscript::node::generic::ptr_type cscript::node::dependent_declaration::get_type() const{
	return type_->query<declaration>()->get_type();
}

cscript::node::generic::ptr_type cscript::node::dependent_declaration::get_declaration() const{
	return type_;
}

cscript::memory::virtual_address::attribute cscript::node::dependent_declaration::get_type_attributes() const{
	return type_->query<declaration>()->get_type_attributes();
}

cscript::type::generic::ptr_type cscript::node::dependent_declaration::get_type_value(){
	return type_->query<declaration>()->get_type_value();
}

std::string cscript::node::dependent_declaration::print_() const{
	return (type_->print() + ", " + id_->print());
}

cscript::node::initialization_declaration::initialization_declaration(const lexer::token::index &index, ptr_type declaration, ptr_type value)
	: declaration(index, declaration, value){}

cscript::node::initialization_declaration::~initialization_declaration(){}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::clone(){
	return std::make_shared<initialization_declaration>(index_, type_->clone(), id_->clone());
}

cscript::object::generic *cscript::node::initialization_declaration::evaluate(){
	auto value = id_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (value == nullptr)
		return common::env::error.set("", type_->get_index());

	type::generic::ptr_type type;
	if (get_type()->is(id::auto_type))
		type = value->get_type();
	else
		type = get_type_value();

	auto *object = allocate(type_->get_index(), type, get_identifier(), get_type_attributes());
	if (common::env::error.has())
		return nullptr;

	if (object == nullptr)
		return common::env::error.set("", type_->get_index());

	common::env::runtime.operand = { nullptr, value };
	return object->evaluate(object::generic::binary_info{ lexer::operator_id::assignment });
}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::get_type() const{
	return type_->query<declaration>()->get_type();
}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::get_identifier() const{
	return type_->query<declaration>()->get_identifier();
}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::get_declaration() const{
	return type_;
}

cscript::node::generic::ptr_type cscript::node::initialization_declaration::get_value() const{
	return id_;
}

cscript::memory::virtual_address::attribute cscript::node::initialization_declaration::get_type_attributes() const{
	return type_->query<declaration>()->get_type_attributes();
}

cscript::type::generic::ptr_type cscript::node::initialization_declaration::get_type_value(){
	return type_->query<declaration>()->get_type_value();
}

std::string cscript::node::initialization_declaration::print_() const{
	return (type_->print() + " = " + id_->print());
}
