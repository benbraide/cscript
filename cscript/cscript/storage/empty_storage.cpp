#include "empty_storage.h"

cscript::storage::empty::empty(generic *parent)
	: parent_(parent){}

cscript::storage::empty::~empty(){}

void cscript::storage::empty::store_function(function_type object){}

cscript::storage::generic *cscript::storage::empty::get_parent(){
	return parent_;
}

cscript::storage::generic *cscript::storage::empty::get_matched(const std::string &key){
	return nullptr;
}

cscript::storage::generic_value *cscript::storage::empty::find(const std::string &key){
	return nullptr;
}

cscript::storage::generic_value &cscript::storage::empty::add(const std::string &key){
	return empty_value_;
}

bool cscript::storage::empty::remove(const std::string &key){
	return false;
}

bool cscript::storage::empty::remove(generic_value &value){
	return false;
}

cscript::storage::generic &cscript::storage::empty::use(const std::string &key, generic_value &value){
	return *this;
}

cscript::storage::generic &cscript::storage::empty::use(generic &storage){
	return *this;
}

cscript::storage::generic &cscript::storage::empty::traverse(traverser_type traverser){
	return *this;
}
