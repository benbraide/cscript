#include "temp_storage.h"

cscript::storage::temp::~temp(){}

void cscript::storage::temp::clear(){
	list_.clear();
}

cscript::object::generic *cscript::storage::temp::add(object::generic::ptr_type object){
	list_.push_back(object);
	return list_.rbegin()->get();
}
