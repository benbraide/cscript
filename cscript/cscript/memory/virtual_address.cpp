#include "virtual_address.h"
#include "../common/env.h"

cscript::memory::virtual_address::virtual_address(){
	none_.info.type = std::make_shared<type::primitive>(type::id::byte);
}

cscript::memory::virtual_address::~virtual_address(){
	for (auto &entry : table::map_){
		if (entry.second.base != nullptr && !CSCRIPT_IS(entry.second.attributes, attribute::no_dealloc))
			delete[] entry.second.base;
	}
}

cscript::memory::virtual_address::value_type cscript::memory::virtual_address::add(size_type size, bool allocate){
	guard_type guard(lock_);
	return add_(size, allocate);
}

cscript::memory::virtual_address::value_type cscript::memory::virtual_address::add(const std::string &value){
	guard_type guard(lock_);

	auto &memory_entry = table::map_.find(add_(static_cast<size_type>(value.size() + 1), false))->second;
	auto &sinfo = (string_list_[memory_entry.value] = string_info{ value + " " });

	memory_entry.info.type = common::env::char_type;
	memory_entry.base = const_cast<char *>(sinfo.value.c_str());

	CSCRIPT_SET(memory_entry.attributes, attribute::string_ | attribute::constant);
	CSCRIPT_REMOVE(memory_entry.attributes, attribute::uninitialized);

	return memory_entry.value;
}

cscript::memory::virtual_address::value_type cscript::memory::virtual_address::allocate(size_type size){
	guard_type guard(lock_);
	auto &entry = table::map_[add_(size, true)];

	entry.info.type = common::env::byte_type;
	CSCRIPT_SET(entry.attributes, attribute::allocated | attribute::byte_aligned);
	CSCRIPT_REMOVE(entry.attributes, attribute::uninitialized);

	return entry.value;
}

bool cscript::memory::virtual_address::shrink(value_type value, size_type size){
	if (size == 0u)
		return false;

	guard_type guard(lock_);
	auto entry = table::map_.find(value);
	if (entry == table::map_.end() || !CSCRIPT_IS(entry->second.attributes, attribute::allocated) || entry->second.size <= size)
		return false;

	delete[] entry->second.base;
	entry->second.size -= size;
	entry->second.base = new char[entry->second.size];
	merge_available_(entry->second.value + entry->second.size, size);

	return true;
}

bool cscript::memory::virtual_address::extend(value_type &value, size_type size){
	if (size == 0u)
		return false;

	guard_type guard(lock_);
	auto entry = table::map_.find(value);
	if (entry == table::map_.end() || !CSCRIPT_IS(entry->second.attributes, attribute::allocated))
		return false;

	auto offset = entry->second.value + entry->second.size;
	if (table::map_.find(offset) == table::map_.end()){
		auto available = available_list_.find(offset);
		if (available != available_list_.end() && size <= available->second){//Can be extended
			auto available_size = available->second;
			available_list_.erase(available);

			if (size < available_size)//Split
				available_list_[offset + size] = (available_size - size);

			delete[] entry->second.base;
			entry->second.size += size;
			entry->second.base = new char[entry->second.size];

			return true;
		}
	}

	auto extended_size = (entry->second.size + size);
	auto attributes = entry->second.attributes;
	auto &new_entry = table::map_[add_(extended_size, true)];

	std::copy(entry->second.base, entry->second.base + entry->second.size, new_entry.base);
	new_entry.attributes = attributes;
	new_entry.info.type = common::env::byte_type;

	remove_(value);
	value = new_entry.value;

	return true;
}

bool cscript::memory::virtual_address::remove(value_type value){
	guard_type guard(lock_);
	return remove_(value);
}

void cscript::memory::virtual_address::remove_string_reference(value_type value){
	guard_type guard(lock_);

	auto entry = string_list_.find(value);
	if (entry == string_list_.end())
		return;

	if (--entry->second.ref_count <= 0){
		string_list_.erase(entry);
		remove_(value);
	}
}

void cscript::memory::virtual_address::add_string_reference(value_type value){
	shared_lock_type guard(lock_);
	auto entry = string_list_.find(value);
	if (entry != string_list_.end())
		++entry->second.ref_count;
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

std::string &cscript::memory::virtual_address::get_string(value_type value){
	shared_lock_type guard(lock_);
	auto entry = string_list_.find(value);
	return (entry == string_list_.end()) ? empty_string_ : entry->second.value;
}

const std::string &cscript::memory::virtual_address::get_empty_string() const{
	return empty_string_;
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

cscript::memory::virtual_address::value_type cscript::memory::virtual_address::add_(size_type size, bool allocate){
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

bool cscript::memory::virtual_address::remove_(value_type value){
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
