#include "pointer_object.h"

cscript::object::basic::basic(memory::virtual_address::value_type memory_value)
	: memory_value_(memory_value){
	if (memory_value_ != 0u)
		common::env::address_space.get_entry(memory_value_).info.object = this;
}

cscript::object::basic::~basic(){
	if (common::env::address_space.get_entry(memory_value_).info.object == this)
		common::env::address_space.remove(memory_value_);
}

cscript::object::generic *cscript::object::basic::remove_reference(){
	return this;
}

cscript::object::generic *cscript::object::basic::clone(){
	return common::env::error.set("Object cannot be cloned");
}

cscript::object::generic *cscript::object::basic::cast(const type::generic *type){
	if (type->is_same(get_memory().info.type.get()) || type->is_any() || type->is_auto())
		return clone();

	return nullptr;
}

cscript::object::generic *cscript::object::basic::ref_cast(const type::generic *type){
	if (type->is_same(get_memory().info.type.get()) || type->is_any() || type->is_auto())
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

		pre_assignment_(*operand);
		if (common::env::error.has())
			return nullptr;

		auto &memory_entry = get_memory();
		auto value = operand->ref_cast(memory_entry.info.type.get());

		if (value == nullptr && (value = operand->cast(memory_entry.info.type.get())) == nullptr)
			return common::env::error.set("Cannot assign value into object");

		common::env::address_space.copy(memory_value_, memory_entry.info.type, value->get_memory_value());
		CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);

		return post_assignment_(*operand);
	}

	if (info.id == lexer::operator_id::comma){
		if (!is_uninitialized())
			return common::env::error.set("Operator does not take specified operands");
		return common::env::get_object_operand();
	}

	return common::env::error.set("Operator does not take specified operands");
}

cscript::object::generic *cscript::object::basic::evaluate(const unary_info &info){
	if (info.left){
		switch (info.id){
		case lexer::operator_id::bitwise_and:
			if (is_lvalue())
				return common::env::temp_storage.add(std::make_shared<pointer>(memory_value_));
			break;
		case lexer::operator_id::relational_not:
			return common::env::error.has() ? nullptr : common::env::temp_storage.add(std::make_shared<primitive::boolean>(
				to_bool() ? type::boolean_value_type::true_ : type::boolean_value_type::false_));
		case lexer::operator_id::call:
			return this;
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

cscript::object::generic &cscript::object::basic::set_memory_value(memory::virtual_address::value_type value){
	if ((memory_value_ = value) != 0u)
		common::env::address_space.get_entry(memory_value_).info.object = this;
	return *this;
}

cscript::memory::virtual_address::value_type cscript::object::basic::get_memory_value(){
	return memory_value_;
}

cscript::memory::virtual_address::entry &cscript::object::basic::get_memory(){
	return common::env::address_space.get_entry(memory_value_);
}

cscript::type::generic::ptr_type cscript::object::basic::get_type(){
	return get_memory().info.type;
}

cscript::storage::generic *cscript::object::basic::get_storage(){
	return get_memory().info.storage;
}

bool cscript::object::basic::is_lvalue(){
	auto &memory = get_memory();
	return (memory.info.storage != nullptr && !CSCRIPT_IS(memory.attributes, memory::virtual_address::attribute::temp));
}

bool cscript::object::basic::is_reference(){
	return CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::ref);
}

bool cscript::object::basic::is_indirect(){
	return CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::indirect);
}

bool cscript::object::basic::is_uninitialized(){
	return CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::uninitialized);
}

bool cscript::object::basic::is_temp(){
	return CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::temp);
}

bool cscript::object::basic::is_constant(){
	return CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::constant);
}

void cscript::object::basic::pre_assignment_(generic &operand){}

cscript::object::generic *cscript::object::basic::post_assignment_(generic &operand){
	return this;
}
