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

cscript::storage::generic_value *cscript::storage::basic::find(const operator_key &key){
	guard_type guard(lock_);

	auto entry = operator_list_.find(key);
	return (entry == operator_list_.end()) ? nullptr : const_cast<value *>(&entry->second);
}

cscript::storage::generic_value &cscript::storage::basic::add(const std::string &key){
	guard_type guard(lock_);
	return table::map_[key];
}

cscript::storage::generic_value &cscript::storage::basic::add(const operator_key &key){
	guard_type guard(lock_);
	return operator_list_[key];
}

bool cscript::storage::basic::remove(const std::string &key){
	guard_type guard(lock_);

	auto entry = table::look_up_(key);
	if (entry != table::map_.end()){
		table::map_.erase(entry);
		return true;
	}

	return false;
}

bool cscript::storage::basic::remove(const operator_key &key){
	guard_type guard(lock_);

	auto entry = operator_list_.find(key);
	if (entry != operator_list_.end()){
		operator_list_.erase(entry);
		return true;
	}

	return false;
}

bool cscript::storage::basic::remove(generic_value &value){
	guard_type guard(lock_);

	for (auto iter = table::map_.begin(); iter != table::map_.end(); ++iter){
		if (&iter->second == &value){
			table::map_.erase(iter);
			return true;
		}
	}

	return false;
}

cscript::storage::generic &cscript::storage::basic::use(const std::string &key, generic_value &value){
	guard_type guard(lock_);
	table::map_[key] = *value.query<storage::value>();
	return *this;
}

cscript::storage::generic &cscript::storage::basic::use(const operator_key &key, generic_value &value){
	guard_type guard(lock_);
	operator_list_[key] = *value.query<storage::value>();
	return *this;
}

cscript::storage::generic &cscript::storage::basic::use(generic &storage){
	guard_type guard(lock_);
	storage.traverse([this](const traversed_key &key, generic_value &value){
		if (key.operator_value== nullptr)
			table::map_[*key.string_value] = *value.query<storage::value>();
		else
			operator_list_[*key.operator_value] = *value.query<storage::value>();

		return true;
	});

	return *this;
}

cscript::storage::generic &cscript::storage::basic::traverse(traverser_type traverser){
	guard_type guard(lock_);
	for (auto &entry : table::map_){
		if (!traverser(traversed_key{ &entry.first }, entry.second))
			break;
	}

	for (auto &entry : operator_list_){
		if (!traverser(traversed_key{ nullptr, &entry.first }, entry.second))
			break;
	}

	return *this;
}
