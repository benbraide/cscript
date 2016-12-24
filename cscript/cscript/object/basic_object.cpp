#include "pointer_object.h"

cscript::object::basic::basic(memory::virtual_address::entry &memory)
	: memory_(memory){
	memory_.object = this;
}

cscript::object::basic::~basic(){
	memory_.address->decrement_ref_count(memory_);
}

cscript::object::generic *cscript::object::basic::clone(){
	return common::env::error.set("Object cannot be cloned");
}

cscript::object::generic *cscript::object::basic::cast(const type::generic *type){
	if (type->is_same(memory_.type.get()) || type->is_any() || type->is_auto())
		return clone();

	return common::env::error.set("Cannot cast to " + type->print());
}

cscript::object::generic *cscript::object::basic::ref_cast(const type::generic *type){
	if (type->is_same(memory_.type.get()) || type->is_any() || type->is_auto())
		return this;

	return common::env::error.set("Cannot cast to ref " + type->print());
}

cscript::object::generic *cscript::object::basic::evaluate(const binary_info &info){
	return common::env::error.set("Operator does not take specified operands");
}

cscript::object::generic *cscript::object::basic::evaluate(const unary_info &info){
	if (info.left && info.id == lexer::operator_id::bitwise_and && is_lvalue())
		return common::env::temp_storage.add(std::make_shared<pointer>(pointer::value_type{ memory_.value, memory_.offset }));

	return common::env::error.set("Operator does not take specified operand");
}

bool cscript::object::basic::to_bool(){
	common::env::error.set("Cannot convert object to boolean value");
	return false;
}

cscript::memory::virtual_address::entry &cscript::object::basic::get_memory(){
	return memory_;
}

cscript::type::generic::ptr_type cscript::object::basic::get_type(){
	return memory_.type;
}

cscript::storage::generic *cscript::object::basic::get_storage(){
	return memory_.storage;
}

bool cscript::object::basic::is_lvalue() const{
	return (memory_.storage != nullptr && !CSCRIPT_IS(memory_.attributes, memory::virtual_address::attribute::temp));
}

bool cscript::object::basic::is_reference() const{
	return CSCRIPT_IS(memory_.attributes, memory::virtual_address::attribute::ref);
}

bool cscript::object::basic::is_indirect() const{
	return CSCRIPT_IS(memory_.attributes, memory::virtual_address::attribute::indirect);
}

bool cscript::object::basic::is_uninitialized() const{
	return CSCRIPT_IS(memory_.attributes, memory::virtual_address::attribute::uninitialized);
}

bool cscript::object::basic::is_constant() const{
	return CSCRIPT_IS(memory_.attributes, memory::virtual_address::attribute::constant);
}
