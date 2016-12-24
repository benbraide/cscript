#include "boolean_object.h"

cscript::object::primitive::boolean::boolean(const type::generic::ptr_type type)
	: boolean(common::env::address_space, type){}

cscript::object::primitive::boolean::boolean(const type::generic::ptr_type type, value_type value)
	: boolean(common::env::address_space, type, value){}

cscript::object::primitive::boolean::boolean(memory::virtual_address &address_space, const type::generic::ptr_type type)
	: basic(address_space.add(type->get_size())){
	memory_.type = type;
}

cscript::object::primitive::boolean::boolean(memory::virtual_address &address_space, const type::generic::ptr_type type, value_type value)
	: basic(address_space.add<value_type>()){
	memory_.type = type;
	memory::pool::write_unchecked(memory_.base, value);
}

cscript::object::primitive::boolean::~boolean(){}
