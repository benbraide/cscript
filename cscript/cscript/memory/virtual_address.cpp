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
	auto offset = table::map_.begin();
	return get_bound_entry_(value, offset);
}

cscript::memory::virtual_address::base_type cscript::memory::virtual_address::get_bound_base(value_type value){
	shared_lock_type guard(lock_);
	auto offset = table::map_.begin();
	return get_bound_base_(value, offset);
}

void cscript::memory::virtual_address::copy(value_type destination, generic_type type, value_type source){
	shared_lock_type guard(lock_);
	auto entry = table::map_.find(destination);
	if (entry != table::map_.end() && entry->second.info.type == type){
		auto &source_entry = get_entry(source);
		if (entry->second.size < source_entry.size)
			copy_unchecked_(destination, source, entry->second.size);
		else
			std::copy(source_entry.base, source_entry.base + source_entry.size, entry->second.base);
	}
	else
		copy_unchecked_(destination, source, type->get_size());
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
	shared_lock_type guard(lock_);
	copy_unchecked_(destination, source, size);
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

cscript::memory::virtual_address::entry cscript::memory::virtual_address::get_bound_entry_(value_type value, table::map_type::iterator &offset){
	for (; offset != table::map_.end(); ++offset){
		if (offset->first == value)
			return offset->second;

		if (offset->first < value && value < (offset->first + offset->second.size)){
			return address_entry{
				offset->second.base + (value - offset->first),
				1u,
				value,
				offset->second.attributes
			};
		}
	}

	return none_;
}

cscript::memory::virtual_address::base_type cscript::memory::virtual_address::get_bound_base_(value_type value, table::map_type::iterator &offset){
	for (; offset != table::map_.end(); ++offset){
		if (offset->first == value)
			return offset->second.base;

		if (offset->first < value && value < (offset->first + offset->second.size))
			return offset->second.base + (value - offset->first);
	}

	return nullptr;
}

void cscript::memory::virtual_address::copy_unchecked_(value_type destination, value_type source, size_type size){
	if (destination == source || size == 0u)
		return;

	auto destination_offset = table::map_.begin(), source_offset = destination_offset;
	for (; size > 0u; --size, ++destination, ++source){
		auto destination_base = get_bound_base_(destination, destination_offset), source_base = get_bound_base_(source, source_offset);
		if (destination_base != nullptr && source_base != nullptr)
			*destination_base = *source_base;
		else
			break;
	}
}
