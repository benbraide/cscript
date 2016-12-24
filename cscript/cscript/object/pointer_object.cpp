#include "pointer_object.h"

cscript::object::pointer::pointer(const value_type &value)
	: pointer(common::env::address_space, value){}

cscript::object::pointer::pointer(memory::virtual_address &address_space, const value_type &value)
	: basic(address_space.add(memory::virtual_address::value_type_size)){
	//memory_.type = type;
	memory::pool::write_ref_unchecked(memory_.base, value);
}

cscript::object::pointer::~pointer(){}

cscript::object::generic *cscript::object::pointer::clone(){
	return common::env::temp_storage.add(std::make_shared<pointer>(memory::pool::convert_unchecked<value_type>(memory_.base)));
}

cscript::object::generic *cscript::object::pointer::evaluate(const binary_info &info){
	return basic::evaluate(info);
}

cscript::object::generic *cscript::object::pointer::evaluate(const unary_info &info){
	return basic::evaluate(info);
}

bool cscript::object::pointer::to_bool(){
	return false;
}
