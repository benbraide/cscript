#include "primitive_object.h"

cscript::object::primitive::generic::generic(memory::virtual_address::entry &memory)
	: basic(memory){}

cscript::object::primitive::generic::~generic(){}

cscript::object::primitive::numeric::numeric(const type::generic::ptr_type type)
	: generic(common::env::address_space.add(type->get_size())){
	memory_.type = type;
}

cscript::object::primitive::numeric::~numeric(){}
