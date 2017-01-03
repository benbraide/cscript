#include "pointer_object.h"

cscript::object::basic::basic(memory::virtual_address::entry &memory)
	: memory_(memory){
	memory_.object = this;
}

cscript::object::basic::~basic(){
	memory_.address->decrement_ref_count(memory_);
}

cscript::object::generic *cscript::object::basic::remove_reference(){
	return this;
}

cscript::object::generic *cscript::object::basic::clone(){
	return common::env::error.set("Object cannot be cloned");
}

cscript::object::generic *cscript::object::basic::cast(const type::generic *type){
	if (type->is_same(memory_.type.get()) || type->is_any() || type->is_auto())
		return clone();

	return nullptr;
}

cscript::object::generic *cscript::object::basic::ref_cast(const type::generic *type){
	if (type->is_same(memory_.type.get()) || type->is_any() || type->is_auto())
		return this;

	return nullptr;
}

cscript::object::generic *cscript::object::basic::evaluate(const binary_info &info){
	if (info.id == lexer::operator_id::assignment){
		if (!is_lvalue() || (is_constant() && !is_uninitialized()))
			return common::env::error.set("Operator does not take specified operands");

		auto operand = common::env::get_object_operand();
		if (operand == nullptr)
			return common::env::error.set("Operator does not take specified operand");

		auto type = get_type();
		auto value = operand->ref_cast(type.get());
		if (value == nullptr && (value = operand->cast(type.get())) == nullptr)
			return common::env::error.set("Cannot assign value into object");

		memory::virtual_address::copy(memory_, value->get_memory());
		CSCRIPT_REMOVE(memory_.attributes, memory::virtual_address::attribute::uninitialized);

		return this;
	}

	return common::env::error.set("Operator does not take specified operands");
}

cscript::object::generic *cscript::object::basic::evaluate(const unary_info &info){
	if (info.left){
		switch (info.id){
		case lexer::operator_id::bitwise_and:
			if (is_lvalue()){
				return common::env::temp_storage.add(std::make_shared<pointer>(
					pointer::value_type{ memory_.address, memory_.value, memory_.offset }));
			}
			break;
		case lexer::operator_id::relational_not:
		{
			auto value = to_bool();
			return common::env::error.has() ? nullptr : common::env::temp_storage.add(std::make_shared<primitive::boolean>(
				common::env::temp_address_space, value ? type::boolean_value_type::true_ : type::boolean_value_type::false_));
		}
		default:
			break;
		}
	}

	return common::env::error.set("Operator does not take specified operand");
}

bool cscript::object::basic::to_bool(){
	common::env::error.set("Cannot convert object to boolean value");
	return false;
}

std::string cscript::object::basic::to_string(){
	common::env::error.set("Cannot convert object to string value");
	return "";
}

std::string cscript::object::basic::echo(){
	common::env::error.set("Cannot echo object");
	return "";
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
