#include "ref_object.h"

cscript::object::ref::ref(const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>()), is_constant_(false){
	get_memory().info.type = type;
}

cscript::object::ref::ref(value_type value, const type::generic::ptr_type type, bool is_constant)
	: basic(common::env::address_space.add<value_type>()), is_constant_(is_constant){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory::pool::write_unchecked(memory_entry.base, value);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::ref::ref(memory::virtual_address::base_type base, const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>(false)), is_constant_(false){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory_entry.base = base;
}

cscript::object::ref::~ref(){}

cscript::object::generic *cscript::object::ref::remove_reference(){
	return get_memory().info.object->remove_reference();
}

cscript::object::generic *cscript::object::ref::clone(){
	return get_memory().info.object->clone();
}

cscript::object::generic *cscript::object::ref::cast(const type::generic *type){
	return get_memory().info.object->cast(type);
}

cscript::object::generic *cscript::object::ref::ref_cast(const type::generic *type){
	return get_memory().info.object->ref_cast(type);
}

cscript::object::generic *cscript::object::ref::evaluate(const binary_info &info){
	if (info.id == lexer::operator_id::assignment && is_uninitialized()){
		auto operand = common::env::get_object_operand();
		if (operand == nullptr)
			return common::env::error.set("Bad initialization expression");

		if (operand->is_constant() && !is_constant())
			return common::env::error.set("Bad initialization expression");

		auto compatible_object = operand->ref_cast(get_type().get());
		if (compatible_object == nullptr || !compatible_object->is_lvalue())
			return common::env::error.set("Bad initialization expression");

		auto &memory_entry = get_memory();
		memory::pool::write_unchecked(memory_entry.base, compatible_object->get_memory_value());
		CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);

		return this;
	}

	switch (info.id){
	case lexer::operator_id::assignment:
	case lexer::operator_id::compound_plus:
	case lexer::operator_id::compound_minus:
	case lexer::operator_id::compound_times:
	case lexer::operator_id::compound_divide:
	case lexer::operator_id::compound_modulus:
	case lexer::operator_id::compound_left_shift:
	case lexer::operator_id::compound_right_shift:
	case lexer::operator_id::compound_bitwise_and:
	case lexer::operator_id::compound_bitwise_or:
	case lexer::operator_id::compound_bitwise_xor:
		break;
	default:
		return get_memory().info.object->evaluate(info);
	}

	if (is_constant_)
		return common::env::error.set("Operator does not take specified operand");

	return get_memory().info.object->evaluate(info);
}

cscript::object::generic *cscript::object::ref::evaluate(const unary_info &info){
	return get_memory().info.object->evaluate(info);
}

bool cscript::object::ref::to_bool(){
	return get_memory().info.object->to_bool();
}

std::string cscript::object::ref::to_string(){
	return get_memory().info.object->to_string();
}

std::string cscript::object::ref::echo(){
	return get_memory().info.object->echo();
}

bool cscript::object::ref::is_constant_ref() const{
	return is_constant_;
}
