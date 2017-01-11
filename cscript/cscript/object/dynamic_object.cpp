#include "dynamic_object.h"

cscript::object::dynamic::dynamic(bool){}

cscript::object::dynamic::dynamic(const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>()){
	get_memory().info.type = type;
}

cscript::object::dynamic::dynamic(ptr_type value, const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>()), object_(value){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory::pool::write_unchecked(memory_entry.base, reinterpret_cast<value_type>(value.get()));
	CSCRIPT_REMOVE(memory_entry.attributes, memory::address_attribute::uninitialized);
	object_->get_memory().info.storage = memory_entry.info.storage;
}

cscript::object::dynamic::dynamic(memory::virtual_address::base_type base, const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>(false)){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory_entry.base = base;
}

cscript::object::dynamic::~dynamic(){}

cscript::object::generic *cscript::object::dynamic::clone(){
	return is_uninitialized() ? nullptr : object_->clone();
}

cscript::object::generic *cscript::object::dynamic::cast(const type::generic *type){
	return is_uninitialized() ? nullptr : object_->cast(type);
}

cscript::object::generic *cscript::object::dynamic::ref_cast(const type::generic *type){
	return (is_uninitialized() || is_constant()) ? nullptr : object_->ref_cast(type);
}

cscript::object::generic *cscript::object::dynamic::const_ref_cast(const type::generic *type){
	return is_uninitialized() ? nullptr : object_->const_ref_cast(type);
}

cscript::object::generic *cscript::object::dynamic::evaluate(const binary_info &info){
	if (info.id == lexer::operator_id::assignment){
		if (!is_lvalue())
			return common::env::error.set("Cannot assign to an rvalue");

		if (is_constant() && !is_uninitialized())
			return common::env::error.set("Cannot modify a constant object");

		auto operand = common::env::get_object_operand();
		if (operand == nullptr)
			return common::env::error.set("void value in expression");

		auto &memory_entry = get_memory();
		auto value = operand->cast(memory_entry.info.type.get());
		if (value == nullptr)
			return common::env::error.set("Object is not compatible with target type");

		if (value->is_reference()){
			value = value->clone();
			if (common::env::error.has())
				return nullptr;

			if (value == nullptr)
				return common::env::error.set("Object could not be copied");
		}

		object_ = value->get_ptr();
		object_->get_memory().info.storage = memory_entry.info.storage;

		memory::pool::write_unchecked(memory_entry.base, reinterpret_cast<value_type>(value));
		CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);

		return this;
	}

	if (is_uninitialized())
		return common::env::error.set("Uninitialized value in expression");

	auto object = object_->remove_reference();
	if (is_constant())
		common::env::runtime.operand.constant_objects.push_back(object);

	return object->evaluate(info);
}

cscript::object::generic *cscript::object::dynamic::evaluate(const unary_info &info){
	if (is_uninitialized()){
		if (info.left && info.id == lexer::operator_id::bitwise_and)
			return common::env::temp_storage.add(common::env::create_pointer(memory_value_, is_constant()));
		return common::env::error.set("Uninitialized value in expression");
	}

	auto object = object_->remove_reference();
	if (is_constant())
		common::env::runtime.operand.constant_objects.push_back(object);

	return object->evaluate(info);
}

bool cscript::object::dynamic::to_bool(){
	return is_uninitialized() ? false : object_->to_bool();
}

std::string cscript::object::dynamic::to_string(){
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return "";
	}

	return object_->to_string();
}

std::string cscript::object::dynamic::echo(){
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return "";
	}

	return object_->echo();
}

cscript::type::generic::ptr_type cscript::object::dynamic::get_type(){
	return is_uninitialized() ? basic::get_type() : object_->get_type();
}

bool cscript::object::dynamic::is_uninitialized(){
	return (object_ == nullptr || basic::is_uninitialized());
}

cscript::object::dynamic_ref::dynamic_ref(memory::virtual_address::value_type memory_value,
	const type::generic::ptr_type type, bool is_constant)
	: dynamic(false), memory_(common::env::address_space.get_bound_entry(memory_value)){
	memory_value_ = memory_value;
	memory_.info.type = type;

	if (memory_.info.object != nullptr)
		object_ = memory_.info.object->get_ptr();

	if (is_constant)
		CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::constant);
}

cscript::object::dynamic_ref::~dynamic_ref(){}

cscript::memory::virtual_address::entry &cscript::object::dynamic_ref::get_memory(){
	return memory_;
}

bool cscript::object::dynamic_ref::is_lvalue(){
	return true;
}
