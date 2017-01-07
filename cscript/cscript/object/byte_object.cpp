#include "byte_object.h"
#include "pointer_object.h"

cscript::object::primitive::byte::byte()
	: basic(common::env::address_space.add<value_type>()){
	get_memory().info.type = common::env::byte_type;
}

cscript::object::primitive::byte::byte(bool){}

cscript::object::primitive::byte::byte(value_type value)
	: basic(common::env::address_space.add<value_type>()){
	auto &memory_entry = get_memory();
	memory_entry.info.type = common::env::byte_type;
	memory::pool::write_unchecked(memory_entry.base, value);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::primitive::byte::byte(memory::virtual_address::base_type base)
	: basic(common::env::address_space.add<value_type>(false)){
	auto &memory_entry = get_memory();
	memory_entry.info.type = common::env::byte_type;
	memory_entry.base = base;
}

cscript::object::primitive::byte::~byte(){}

cscript::object::generic *cscript::object::primitive::byte::clone(){
	return common::env::temp_storage.add(std::make_shared<byte>(get_value_()));
}

cscript::object::generic *cscript::object::primitive::byte::cast(const type::generic *type){
	switch (type->get_id()){
	case type::id::char_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::char_type, get_value<char>()));
	case type::id::uchar:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::uchar_type, get_value<unsigned char>()));
	default:
		break;
	}

	if (type->is_pointer() && get_type()->is_integral()){
		auto value = get_value<memory::virtual_address::value_type>();
		auto base_type = type->query<cscript::type::pointer>()->get_value();
		return common::env::temp_storage.add(std::make_shared<pointer>(value, base_type));
	}

	return get_type()->has_conversion(type) ? clone() : basic::cast(type);
}

cscript::object::generic *cscript::object::primitive::byte::evaluate(const binary_info &info){
	auto operand = common::env::get_object_operand();
	if (operand == nullptr || (operand = operand->remove_reference()) == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	if (operand->get_type()->get_id() != type::id::byte)
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

std::string cscript::object::primitive::byte::echo(){
	return common::env::to_hex(get_value_());
}

cscript::object::primitive::byte::value_type cscript::object::primitive::byte::get_value_(){
	return common::env::address_space.convert<value_type>(memory_value_, get_type());
}

cscript::object::primitive::boolean::value_type cscript::object::primitive::byte::compare_(generic &operand, bool equality){
	return ((get_value_() == operand.query<byte>()->get_value_()) == equality) ? boolean::value_type::true_ : boolean::value_type::false_;
}

cscript::object::primitive::byte_ref::byte_ref(memory::virtual_address::value_type memory_value, bool is_constant)
	: byte(false), memory_(common::env::address_space.get_bound_entry(memory_value)){
	memory_value_ = memory_value;
	memory_.info.type = common::env::bool_type;
	if (is_constant)
		CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::constant);
}

cscript::object::primitive::byte_ref::~byte_ref(){}

cscript::memory::virtual_address::entry &cscript::object::primitive::byte_ref::get_memory(){
	return memory_;
}

bool cscript::object::primitive::byte_ref::is_lvalue(){
	return true;
}
