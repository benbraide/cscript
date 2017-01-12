#include "boolean_object.h"

cscript::object::primitive::boolean::boolean()
	: basic(common::env::address_space.add<value_type>()){
	get_memory().info.type = common::env::bool_type;
}

cscript::object::primitive::boolean::boolean(bool){}

cscript::object::primitive::boolean::boolean(value_type value)
	: basic(common::env::address_space.add<value_type>()){
	auto &memory_entry = get_memory();
	memory_entry.info.type = common::env::bool_type;
	memory::pool::write_unchecked(memory_entry.base, value);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::primitive::boolean::boolean(memory::virtual_address::base_type base)
	: basic(common::env::address_space.add<value_type>(false)){
	auto &memory_entry = get_memory();
	memory_entry.info.type = common::env::bool_type;
	memory_entry.base = base;
}

cscript::object::primitive::boolean::~boolean(){}

cscript::object::generic *cscript::object::primitive::boolean::clone(){
	return is_uninitialized() ? nullptr : common::env::temp_storage.add(std::make_shared<boolean>(get_value_()));
}

cscript::object::generic *cscript::object::primitive::boolean::evaluate(const binary_info &info){
	auto operand = common::env::get_object_operand();
	if (operand == nullptr || (operand = operand->remove_reference()) == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	if (operand->get_type()->get_id() != type::id::bool_)
		return basic::evaluate(info);

	switch (info.id){
	case lexer::operator_id::equality:
		return common::env::temp_storage.add(std::make_shared<boolean>(compare_(*operand, true)));
	case lexer::operator_id::inverse_equality:
		return common::env::temp_storage.add(std::make_shared<boolean>(compare_(*operand, false)));
	default:
		break;
	}

	return basic::evaluate(info);
}

void cscript::object::primitive::boolean::initialize(){
	auto &memory_entry = get_memory();
	memory::pool::write_unchecked(get_memory().base, value_type::nil);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

bool cscript::object::primitive::boolean::to_bool(){
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return false;
	}

	return (get_value_() == value_type::true_);
}

std::string cscript::object::primitive::boolean::echo(){
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return "";
	}

	switch (get_value_()){
	case value_type::true_:
		return "true";
	case value_type::false_:
		return "false";
	default:
		break;
	}

	return "indeterminate";
}

cscript::object::primitive::boolean::value_type cscript::object::primitive::boolean::get_value_(){
	return common::env::address_space.convert<value_type>(memory_value_, get_type());
}

cscript::object::primitive::boolean::value_type cscript::object::primitive::boolean::compare_(generic &operand, bool equality){
	return ((get_value_() == operand.query<boolean>()->get_value_()) == equality) ? value_type::true_ : value_type::false_;
}

cscript::object::primitive::boolean_ref::boolean_ref(memory::virtual_address::value_type memory_value, bool is_constant)
	: boolean(false), memory_(common::env::address_space.get_bound_entry(memory_value)){
	memory_value_ = memory_value;
	memory_.info.type = common::env::bool_type;
	if (is_constant)
		CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::constant);
}

cscript::object::primitive::boolean_ref::~boolean_ref(){}

cscript::memory::virtual_address::entry &cscript::object::primitive::boolean_ref::get_memory(){
	return memory_;
}

bool cscript::object::primitive::boolean_ref::is_lvalue(){
	return true;
}
