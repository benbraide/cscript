#include "virtual_address.h"

cscript::memory::virtual_address::virtual_address(pool &pool, attribute attributes)
	: pool_(pool), attributes_(attributes){}

cscript::memory::virtual_address::~virtual_address(){
	for (auto &entry : list_)
		pool_.remove(entry.base);
}

cscript::memory::virtual_address::entry &cscript::memory::virtual_address::add(size_type size){
	guard_type guard(lock_);

	auto base = pool_.add(size);
	if (base == nullptr)//Failed to allocate memory
		return none_;

	iterator_type available;
	if (last_ != nullptr && (available = find_available_(size)) != list_.end()){//Use available
		if (last_ != &*available && available->size != size){//Unused space
			list_.insert(std::next(available), entry{
				this,
				nullptr,
				available->size - size,
				(available->value + size)
			});
		}

		available->base = base;
		available->size = size;
		available->attributes = attributes_;

		return *available;
	}
	
	list_.push_back(entry{
		this,
		base,
		size,
		(last_ == nullptr) ? 1u : (last_->value + last_->size),
		attributes_
	});

	return *(last_ = &*list_.rbegin());
}

bool cscript::memory::virtual_address::remove(value_type value){
	guard_type guard(lock_);

	auto entry = find_(value);
	if (entry == list_.end() || !pool_.remove(entry->base))
		return false;

	if (entry != list_.begin()){//Check if previous slot is empty
		auto previous_entry = std::prev(entry);
		if (previous_entry->base == nullptr)//Empty
			previous_entry->size += entry->size;

		if (last_ == &*entry)
			last_ = &*previous_entry;
	}
	else if (last_ == &*entry)
		last_ = nullptr;

	list_.erase(entry);
	return true;
}

bool cscript::memory::virtual_address::update_ref_count(value_type value, size_type count, bool increment){
	guard_type guard(lock_);

	auto entry = find_(value);
	if (entry == list_.end())
		return false;

	switch (pool_.update_ref_count(entry->base, count, increment)){
	case 0:
		return false;
	case 2://
		list_.erase(entry);
		break;
	default:
		break;
	}

	return true;
}

bool cscript::memory::virtual_address::decrement_ref_count(value_type value){
	return update_ref_count(value, -1, false);
}

bool cscript::memory::virtual_address::increment_ref_count(value_type value){
	return update_ref_count(value, 1, true);
}

bool cscript::memory::virtual_address::is_none(const entry &entry) const{
	return (&entry == &none_);
}

bool cscript::memory::virtual_address::has(value_type value) const{
	guard_type guard(lock_);
	return (find_(value) != list_.end());
}

cscript::memory::virtual_address::entry &cscript::memory::virtual_address::get_entry(value_type value){
	guard_type guard(lock_);
	auto entry = find_(value);
	return (entry == list_.end()) ? none_ : *entry;
}

cscript::memory::virtual_address::iterator_type cscript::memory::virtual_address::find_(value_type value){
	for (auto entry = list_.begin(); entry != list_.end(); ++entry){
		if (entry->value <= value && (entry->value + entry->size) < value)
			return entry;
	}

	return list_.end();
}

cscript::memory::virtual_address::const_iterator_type cscript::memory::virtual_address::find_(value_type value) const{
	for (auto entry = list_.begin(); entry != list_.end(); ++entry){
		if (entry->value <= value && (entry->value + entry->size) < value)
			return entry;
	}

	return list_.end();
}

cscript::memory::virtual_address::iterator_type cscript::memory::virtual_address::find_available_(size_type size){
	for (auto entry = list_.begin(); entry != list_.end(); ++entry){
		if (entry->base == nullptr && size <= entry->size)
			return entry;
	}

	return list_.end();
}
