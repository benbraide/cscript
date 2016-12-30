#include "basic_storage.h"

cscript::storage::basic::basic(generic *parent)
	: parent_(parent){}

cscript::storage::basic::~basic(){}

cscript::storage::generic *cscript::storage::basic::get_parent(){
	return parent_;
}

cscript::storage::generic *cscript::storage::basic::get_matched(const std::string &key){
	return (parent_ == nullptr) ? nullptr : parent_->get_matched(key);
}

cscript::storage::generic_value *cscript::storage::basic::find(const std::string &key){
	guard_type guard(lock_);

	auto entry = table::look_up_(key);
	return (entry == table::map_.end()) ? nullptr : const_cast<value *>(&entry->second);
}

cscript::storage::generic_value &cscript::storage::basic::add(const std::string &key){
	guard_type guard(lock_);

	auto &value = table::map_[key];
	table::compile_();

	return value;
}

bool cscript::storage::basic::remove(const std::string &key){
	guard_type guard(lock_);

	auto entry = table::look_up_(key);
	if (entry != table::map_.end()){
		table::map_.erase(entry);
		table::compile_();
		return true;
	}

	return false;
}

bool cscript::storage::basic::remove(generic_value &value){
	guard_type guard(lock_);

	for (auto iter = table::map_.begin(); iter != table::map_.end(); ++iter){
		if (&iter->second == &value){
			table::map_.erase(iter);
			table::compile_();
			return true;
		}
	}

	return false;
}

cscript::storage::generic &cscript::storage::basic::use(const std::string &key, generic_value &value){
	guard_type guard(lock_);

	table::map_[key] = *value.query<storage::value>();
	table::compile_();

	return *this;
}

cscript::storage::generic &cscript::storage::basic::use(generic &storage){
	guard_type guard(lock_);
	storage.traverse([this](const std::string &key, generic_value &value){
		table::map_[key] = *value.query<storage::value>();
		table::compile_();
		return true;
	});

	return *this;
}

cscript::storage::generic &cscript::storage::basic::traverse(traverser_type traverser){
	guard_type guard(lock_);
	for (auto &entry : table::map_){
		if (!traverser(entry.first, entry.second))
			break;
	}

	return *this;
}
