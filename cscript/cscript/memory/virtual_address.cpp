#include "virtual_address.h"

cscript::memory::virtual_address::~virtual_address(){
	for (auto &list : list_){
		for (auto &entry : list)
			delete[] entry.base;
	}
}

cscript::memory::virtual_address::entry &cscript::memory::virtual_address::add(size_type size, attribute set, attribute remove){
	guard_type guard(lock_);

	auto base = new char[size];
	if (base == nullptr)//Failed to allocate memory
		return none_;

	find_info info{};
	find_available_(size, info);

	if (info.list != nullptr && info.iterator != info.list->end()){//Use available
		if (&*info.iterator != &info.list->back() && info.iterator->size != size){//Unused space
			info.list->insert(std::next(info.iterator), entry{
				this,
				nullptr,
				info.iterator->size - size,
				0u,
				(info.iterator->value + size),
				info.iterator->offset
			});
		}

		info.iterator->base = base;
		info.iterator->size = size;
		info.iterator->ref_count = 1;
		info.iterator->attributes = set;

		return *info.iterator;
	}

	size_type offset;
	info.list = get_next_list_(offset);

	info.list->push_back(entry{
		this,
		base,
		size,
		1u,
		info.list->empty() ? 1u : (info.list->rbegin()->value + info.list->rbegin()->size),
		offset,
		set
	});

	return *info.list->rbegin();
}

bool cscript::memory::virtual_address::remove(const entry &entry){
	guard_type guard(lock_);
	if (&entry == &none_)
		return false;

	find_info info;
	find_(entry, info);
	remove_(info);

	return true;
}

bool cscript::memory::virtual_address::update_ref_count(const entry &entry, size_type count, bool increment){
	guard_type guard(lock_);
	if (&entry == &none_)
		return false;

	find_info info;
	find_(entry, info);

	if (info.list == nullptr || info.iterator == info.list->end())
		return false;

	if (info.iterator->ref_count == 0u || --info.iterator->ref_count == 0u)
		remove_(info);

	return true;
}

bool cscript::memory::virtual_address::decrement_ref_count(const entry &entry){
	return update_ref_count(entry, -1, false);
}

bool cscript::memory::virtual_address::increment_ref_count(const entry &entry){
	return update_ref_count(entry, 1, true);
}

bool cscript::memory::virtual_address::is_none(const entry &entry) const{
	return (&entry == &none_);
}

cscript::memory::virtual_address::entry &cscript::memory::virtual_address::get_entry(const value_info &info){
	auto list = get_list_(info.offset);
	for (auto list_entry = list->begin(); list_entry != list->end(); ++list_entry){
		if (info.value >= list_entry->value && info.value < (list_entry->value + list_entry->size))
			return *list_entry;
	}

	return none_;
}

bool cscript::memory::virtual_address::copy(const entry &source, const entry &destination){
	if (source.base == destination.base || destination.size < source.size)
		return false;

	std::copy(source.base, source.base + source.size, destination.base);
	return true;
}

void cscript::memory::virtual_address::copy_unchecked(const entry &source, const entry &destination){

}

void cscript::memory::virtual_address::remove_(find_info &info){
	delete[] info.iterator->base;
	info.iterator->base = nullptr;

	if (info.iterator != info.list->begin()){//Check if previous slot is empty
		auto previous_entry = std::prev(info.iterator);
		if (previous_entry->base == nullptr)//Empty
			previous_entry->size += info.iterator->size;

		info.list->erase(info.iterator);
	}
	else//Indicate empty slot
		info.iterator->base = nullptr;
}

void cscript::memory::virtual_address::find_(const entry &entry, find_info &info){
	info.list = get_list_(entry.offset);
	if (info.list == nullptr)
		return;

	for (auto list_entry = info.list->begin(); list_entry != info.list->end(); ++list_entry){
		if (entry.value >= list_entry->value && entry.value < (list_entry->value + list_entry->size)){
			info.iterator = list_entry;
			return;
		}
	}

	info.iterator = info.list->end();
}

void cscript::memory::virtual_address::find_available_(size_type size, find_info &info){
	for (auto &next : list_){//Traverse entire list
		for (auto entry = next.begin(); entry != next.end(); ++entry){//Traverse entries
			if (entry->base != nullptr || entry->size < size)
				continue;//Slot used | insufficient space

			info.iterator = entry;
			info.list = &next;

			break;
		}
	}
}

cscript::memory::virtual_address::list_type *cscript::memory::virtual_address::get_list_(size_type offset){
	return (offset < list_.size()) ? &*std::next(list_.begin(), offset) : nullptr;
}

cscript::memory::virtual_address::list_type *cscript::memory::virtual_address::get_next_list_(size_type &offset){
	offset = 0;
	if (!list_.empty()){
		for (auto &next : list_){
			if (next.size() < next.max_size())
				return &next;

			++offset;
		}
	}

	list_.push_back(list_type{});
	return &*list_.rbegin();
}

cscript::memory::temp_virtual_address::~temp_virtual_address(){ }

void cscript::memory::temp_virtual_address::find_available_(size_type size, find_info &info){}
