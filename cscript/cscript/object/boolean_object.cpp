#include "boolean_object.h"

cscript::object::primitive::boolean::boolean()
	: boolean(common::env::address_space){}

cscript::object::primitive::boolean::boolean(value_type value)
	: boolean(common::env::address_space, value){}

cscript::object::primitive::boolean::boolean(memory::virtual_address &address_space)
	: basic(address_space.add(common::env::bool_type->get_size())){
	memory_.type = common::env::bool_type;
}

cscript::object::primitive::boolean::boolean(memory::virtual_address &address_space, value_type value)
	: basic(address_space.add<value_type>()){
	memory_.type = common::env::bool_type;
	memory::pool::write_unchecked(memory_.base, value);
}

cscript::object::primitive::boolean::boolean(memory::virtual_address::entry &parent)
	: basic(parent.address->add(parent, common::env::bool_type->get_size())){
	memory_.type = common::env::bool_type;
}

cscript::object::primitive::boolean::~boolean(){}

cscript::object::generic *cscript::object::primitive::boolean::clone(){
	auto value = std::make_shared<boolean>(common::env::temp_address_space);

	common::env::temp_storage.add(value);
	memory::virtual_address::copy(memory_, value->get_memory());

	return value.get();
}

cscript::object::generic *cscript::object::primitive::boolean::evaluate(const binary_info &info){
	auto operand = common::env::get_object_operand();
	if (operand == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	if (operand->get_type()->get_id() != type::id::bool_)
		return basic::evaluate(info);

	switch (info.id){
	case lexer::operator_id::equality:
		return common::env::temp_storage.add(std::make_shared<boolean>(common::env::temp_address_space,
			(get_value_() == operand->query<boolean>()->get_value_()) ? value_type::true_ : value_type::false_));
	case lexer::operator_id::inverse_equality:
		return common::env::temp_storage.add(std::make_shared<boolean>(common::env::temp_address_space,
			(get_value_() != operand->query<boolean>()->get_value_()) ? value_type::true_ : value_type::false_));
	default:
		break;
	}

	return basic::evaluate(info);
}

bool cscript::object::primitive::boolean::to_bool(){
	return (memory::pool::convert_unchecked<value_type>(memory_.base) == value_type::true_);
}

cscript::object::primitive::boolean::value_type cscript::object::primitive::boolean::get_value_(){
	return memory::pool::convert_unchecked<value_type>(memory_.base);
}
