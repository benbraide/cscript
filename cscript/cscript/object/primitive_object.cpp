#include "primitive_object.h"

cscript::object::primitive::generic::generic(memory::virtual_address::entry &memory)
	: basic(memory){}

cscript::object::primitive::generic::~generic(){}

cscript::object::primitive::numeric::numeric(memory::virtual_address &target_memory, memory::virtual_address::size_type size)
	: generic(target_memory.add(size)){}

cscript::object::primitive::numeric::~numeric(){}
