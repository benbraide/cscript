#include "ref_object.h"

cscript::object::ref::ref(value_type &value, bool is_constant)
	: basic(value), value_(value), is_constant_(is_constant){
	value_.address->increment_ref_count(memory_);
}

cscript::object::ref::~ref(){
	value_.address->decrement_ref_count(memory_);
}

cscript::object::generic *cscript::object::ref::clone(){
	return value_.object->clone();
}

cscript::object::generic *cscript::object::ref::cast(const type::generic *type){
	return value_.object->cast(type);
}

cscript::object::generic *cscript::object::ref::ref_cast(const type::generic *type){
	return value_.object->ref_cast(type);
}

cscript::object::generic *cscript::object::ref::evaluate(const binary_info &info){
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
		return value_.object->evaluate(info);
	}

	if (is_constant())
		return common::env::error.set("Operator does not take specified operand");

	return value_.object->evaluate(info);
}

cscript::object::generic *cscript::object::ref::evaluate(const unary_info &info){
	return value_.object->evaluate(info);
}

bool cscript::object::ref::to_bool(){
	return value_.object->to_bool();
}

cscript::memory::virtual_address::entry &cscript::object::ref::get_memory(){
	return value_;
}

cscript::type::generic::ptr_type cscript::object::ref::get_type(){
	return value_.object->get_type();
}

cscript::storage::generic *cscript::object::ref::get_storage(){
	return value_.object->get_storage();
}

bool cscript::object::ref::is_lvalue() const{
	return true;
}

bool cscript::object::ref::is_reference() const{
	return true;
}

bool cscript::object::ref::is_indirect() const{
	return value_.object->is_indirect();
}

bool cscript::object::ref::is_uninitialized() const{
	return value_.object->is_uninitialized();
}

bool cscript::object::ref::is_constant() const{
	return (is_constant_ || value_.object->is_constant());
}
