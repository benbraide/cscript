#include "ref_object.h"

cscript::object::ref::ref(const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>()), is_constant_(false), value_(nullptr){
	memory_.type = type;
	memory::pool::convert_unchecked<value_type>(memory_.base) = 0;
}

cscript::object::ref::ref(entry_type &value, bool is_constant)
	: basic(common::env::address_space.add<value_type>()), is_constant_(is_constant), value_(&value){
	memory_.type = value.type;
	value.address->increment_ref_count(value);

	CSCRIPT_REMOVE(memory_.attributes, memory::virtual_address::attribute::uninitialized);
	memory::pool::convert_unchecked<value_type>(memory_.base) = reinterpret_cast<value_type>(&value);
}

cscript::object::ref::~ref(){
	if (value_ != nullptr)
		value_->address->decrement_ref_count(*value_);
}

cscript::object::generic *cscript::object::ref::remove_reference(){
	return value_->object->remove_reference();
}

cscript::object::generic *cscript::object::ref::clone(){
	return value_->object->clone();
}

cscript::object::generic *cscript::object::ref::cast(const type::generic *type){
	return value_->object->cast(type);
}

cscript::object::generic *cscript::object::ref::ref_cast(const type::generic *type){
	return value_->object->ref_cast(type);
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

		CSCRIPT_REMOVE(memory_.attributes, memory::virtual_address::attribute::uninitialized);
		memory::pool::convert_unchecked<value_type>(memory_.base) =
			reinterpret_cast<value_type>(value_ = &compatible_object->get_memory());
		value_->address->increment_ref_count(*value_);

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
		return value_->object->evaluate(info);
	}

	if (is_constant_)
		return common::env::error.set("Operator does not take specified operand");

	return value_->object->evaluate(info);
}

cscript::object::generic *cscript::object::ref::evaluate(const unary_info &info){
	return value_->object->evaluate(info);
}

bool cscript::object::ref::to_bool(){
	return value_->object->to_bool();
}

std::string cscript::object::ref::to_string(){
	return value_->object->to_string();
}

std::string cscript::object::ref::echo(){
	return value_->object->echo();
}

bool cscript::object::ref::is_constant_ref() const{
	return is_constant_;
}
