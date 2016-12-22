#include "basic_object.h"

cscript::object::basic::basic(memory::virtual_address::entry &memory)
	: memory_(memory){
	memory_.object = this;
}

cscript::object::basic::~basic(){
	memory_.address->decrement_ref_count(memory_);
}

cscript::memory::virtual_address::entry &cscript::object::basic::get_memory(){
	return memory_;
}
