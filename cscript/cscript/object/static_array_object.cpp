#include "static_array_object.h"

#include "../type/primitive_type.h"
#include "../common/env.h"

cscript::object::primitive::static_array::static_array(){}

cscript::object::primitive::static_array::static_array(const type::generic::ptr_type type)
	: basic(common::env::address_space.add(type->get_size())){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	allocate_(memory_entry.base + 1, type);
}

cscript::object::primitive::static_array::static_array(memory::virtual_address::base_type base,
	const type::generic::ptr_type type)
	: basic(common::env::address_space.add(type->get_size(), false)){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory_entry.base = base;
	allocate_(base + 1, type);
}

cscript::object::primitive::static_array::~static_array(){}

cscript::object::generic *cscript::object::primitive::static_array::cast(const type::generic *type){
	if (type->is_pointer()){
		auto base_type = get_type()->query<cscript::type::array>()->get_value();
		if (type->query<cscript::type::pointer>() == nullptr || base_type->is_same(type->remove_pointer()))
			return common::env::temp_storage.add(std::make_shared<pointer>(entries_[0]->get_memory_value(), base_type));
	}

	return nullptr;
}

cscript::object::generic *cscript::object::primitive::static_array::evaluate(const binary_info &info){
	auto operand = common::env::get_object_operand();
	if (operand == nullptr || (operand = operand->remove_reference()) == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	auto operand_type = operand->get_type();
	if (operand_type == nullptr)
		return basic::evaluate(info);

	if (info.id == lexer::operator_id::index && operand_type->is_integral()){
		auto value = operand->query<primitive::numeric>()->get_value<int>();
		if (value < 0 || static_cast<int>(entries_.size()) <= value)
			return common::env::error.set("Index is out of bounds");

		return entries_[value].get();
	}

	return basic::evaluate(info);
}

cscript::object::generic *cscript::object::primitive::static_array::evaluate(const unary_info &info){
	if (info.left && info.id == lexer::operator_id::sizeof_){
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::uint_type, get_type()->get_size() - 1u));
	}

	return basic::evaluate(info);
}

std::string cscript::object::primitive::static_array::echo(){
	std::string value, entry_string;
	for (auto entry : entries_){
		entry_string = entry->echo();
		if (common::env::error.has())
			return "";

		if (value.empty())
			value = entry_string;
		else//Append
			value += (", " + entry_string);
	}

	return ("[" + value + "]");
}

bool cscript::object::primitive::static_array::is_constant(){
	return (CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::final_) ||
		common::env::is_constant(*this));
}

bool cscript::object::primitive::static_array::is_constant_target(){
	return CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::constant);
}

const cscript::object::primitive::static_array::list_type &cscript::object::primitive::static_array::get_entries() const{
	return entries_;
}

void cscript::object::primitive::static_array::pre_assignment_(object::generic &operand){
	common::env::error.set("Cannot assign value to an array");
}

std::nullptr_t cscript::object::primitive::static_array::allocate_(memory::virtual_address::base_type base,
	const type::generic::ptr_type type){
	auto base_type = type->query<cscript::type::array>()->get_value();
	auto base_size = base_type->get_size();
	auto size = ((type->get_size() - 1) / base_size);

	entries_.reserve(size);
	for (; size > 0u; --size){
		auto entry = base_type->create(base, base_type);
		if (common::env::error.has())
			break;

		if (entry == nullptr)
			return common::env::error.set("Failed to allocate array");

		entries_.push_back(entry);
		entry->get_memory().info.storage = this;
		entry->initialize();

		base += base_size;
	}

	return nullptr;
}

cscript::object::primitive::static_array_ref::static_array_ref(memory::virtual_address::value_type memory_value,
	const type::generic::ptr_type type, bool is_constant)
	: memory_(common::env::address_space.get_bound_entry(memory_value)){
	memory_value_ = memory_value;
	memory_.info.type = type;

	if (is_constant)
		CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::final_);

	auto array_object = (memory_.info.object == nullptr) ? nullptr : memory_.info.object->query<static_array>();
	if (array_object == nullptr)
		common::env::error.set("Invalid array reference");
	else//Copy entries
		entries_ = array_object->get_entries();
}

cscript::object::primitive::static_array_ref::~static_array_ref(){}

cscript::memory::virtual_address::entry &cscript::object::primitive::static_array_ref::get_memory(){
	return memory_;
}

bool cscript::object::primitive::static_array_ref::is_lvalue(){
	return true;
}
