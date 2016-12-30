#include "storage_value.h"

cscript::storage::value::value(){}

cscript::storage::value::value(generic &storage){
	info_.storage = &storage;
}

cscript::storage::value::value(type_ptr type){
	info_.type = type;
}

cscript::storage::value::value(object_ptr object){
	info_.object = object;
}

void cscript::storage::value::set(generic &storage){
	info_.storage = &storage;
}

void cscript::storage::value::set(type_ptr type){
	info_.type = type;
}

void cscript::storage::value::set(object_ptr object){
	info_.object = object;
}

cscript::storage::generic *cscript::storage::value::get_storage(){
	return (info_.storage == nullptr) ? info_.type->query<generic>() : info_.storage;
}

cscript::storage::generic_value::type_ptr cscript::storage::value::get_type(){
	return info_.type;
}

cscript::storage::generic_value::object_type *cscript::storage::value::get_object(){
	return info_.object.get();
}

bool cscript::storage::value::is_storage() const{
	return (info_.storage != nullptr);
}

bool cscript::storage::value::is_type() const{
	return (info_.type != nullptr);
}
