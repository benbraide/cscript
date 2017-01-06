#include "virtual_address.h"
#include "../common/env.h"

cscript::memory::virtual_address::virtual_address(){
	none_.info.type = common::env::byte_type;
}

cscript::memory::virtual_address::~virtual_address(){
	for (auto &entry : table::map_){
		if (entry.second.base != nullptr && !CSCRIPT_IS(entry.second.attributes, attribute::no_dealloc))
			delete[] entry.second.base;
	}
}

cscript::memory::virtual_address::value_type cscript::memory::virtual_address::add(size_type size, bool allocate){
	guard_type guard(lock_);

	auto base = allocate ? new char[size] : nullptr;
	if (allocate && base == nullptr)//Failed to allocate memory
		return 0ull;

	auto attributes = attribute::uninitialized;
	if (!allocate)
		CSCRIPT_SET(attributes, attribute::no_dealloc);

	auto available = find_available_(size);
	if (available == 0u){
		available = next_;
		next_ += size;
	}
	else//Remove from list
		available_list_.erase(available);
	
	table::map_[available] = address_entry{
		base,
		size,
		available,
		attributes
	};

	return available;
}

bool cscript::memory::virtual_address::remove(value_type value){
	guard_type guard(lock_);

	auto entry = table::map_.find(value);
	if (entry == table::map_.end())
		return false;

	if (entry->second.base != nullptr && !CSCRIPT_IS(entry->second.attributes, attribute::no_dealloc))
		delete[] entry->second.base;

	if (!CSCRIPT_IS(entry->second.attributes, attribute::is_child))
		merge_available_(value, entry->second.size);

	table::map_.erase(entry);
	return true;
}

cscript::memory::virtual_address::entry &cscript::memory::virtual_address::get_entry(value_type value){
	shared_lock_type guard(lock_);
	auto entry = table::map_.find(value);
	return (entry == table::map_.end()) ? none_ : entry->second;
}

cscript::memory::virtual_address::entry cscript::memory::virtual_address::get_bound_entry(value_type value){
	shared_lock_type guard(lock_);
	for (auto &entry : table::map_){
		if (entry.first == value)
			return entry.second;

		if (entry.first < value && value < (entry.first + entry.second.size)){
			return address_entry{
				entry.second.base + (value - entry.first),
				1u,
				value,
				entry.second.attributes
			};
		}
	}

	return none_;
}

bool cscript::memory::virtual_address::copy(value_type destination, value_type source){
	if (destination == source)
		return false;

	auto &destination_entry = get_entry(destination), &source_entry = get_entry(source);
	if (destination_entry.size < source_entry.size)
		return false;

	std::copy(source_entry.base, source_entry.base + source_entry.size, destination_entry.base);
	return true;
}

void cscript::memory::virtual_address::copy_unchecked(value_type destination, value_type source, size_type size){
	if (destination == source || size == 0u)
		return;

	for (; size > 0u; --size, ++destination, ++source){
		auto destination_entry = get_bound_entry(destination), source_entry = get_bound_entry(source);
		*destination_entry.base = *source_entry.base;
	}
}

cscript::memory::virtual_address::value_type cscript::memory::virtual_address::find_available_(size_type size){
	for (auto &entry : available_list_){
		if (size < entry.second){//Split
			available_list_[entry.first + size] = (entry.second - size);
			return entry.first;
		}

		if (size == entry.second)
			return entry.first;
	}

	return 0u;
}

void cscript::memory::virtual_address::merge_available_(value_type value, size_type size){
	for (auto &entry : available_list_){
		if ((entry.first + entry.second) == value){//Merge
			entry.second += size;
			return;
		}
	}

	available_list_[value] = size;
}
