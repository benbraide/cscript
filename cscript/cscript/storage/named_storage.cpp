#include "named_storage.h"

cscript::storage::named::named(const std::string &name, generic *parent)
	: basic(parent), name_(name){}

cscript::storage::named::~named(){}

cscript::storage::generic *cscript::storage::named::get_matched(const std::string &key){
	if (key == name_)
		return this;

	return (parent_ == nullptr) ? nullptr : parent_->get_matched(key);
}

const std::string &cscript::storage::named::get_name() const{
	return name_;
}
