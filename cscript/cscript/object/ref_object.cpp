#include "ref_object.h"
#include "pointer_object.h"

cscript::object::ref::ref(const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>()), is_constant_(false){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	CSCRIPT_SET(memory_entry.attributes, memory::address_attribute::ref);
}

cscript::object::ref::ref(value_type value, const type::generic::ptr_type type, bool is_constant)
	: basic(common::env::address_space.add<value_type>()), is_constant_(is_constant){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory::pool::write_unchecked(memory_entry.base, value);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::address_attribute::uninitialized);
	CSCRIPT_SET(memory_entry.attributes, memory::address_attribute::ref);
}

cscript::object::ref::ref(memory::virtual_address::base_type base, const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>(false)), is_constant_(false){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory_entry.base = base;
	CSCRIPT_SET(memory_entry.attributes, memory::address_attribute::ref);
}

cscript::object::ref::~ref(){}

cscript::object::generic *cscript::object::ref::remove_reference(){
	return get_object_()->remove_reference();
}

cscript::object::generic *cscript::object::ref::clone(){
	return is_uninitialized() ? nullptr : get_object_()->clone();
}

cscript::object::generic *cscript::object::ref::cast(const type::generic *type){
	return is_uninitialized() ? nullptr : get_object_()->cast(type);
}

cscript::object::generic *cscript::object::ref::ref_cast(const type::generic *type){
	return (is_uninitialized() || is_constant()) ? nullptr : get_object_()->ref_cast(type);
}

cscript::object::generic *cscript::object::ref::const_ref_cast(const type::generic *type){
	return is_uninitialized() ? nullptr : get_object_()->ref_cast(type);
}

cscript::object::generic *cscript::object::ref::evaluate(const binary_info &info){
	if (info.id == lexer::operator_id::assignment && is_uninitialized())
		return basic::evaluate(info);

	if (is_uninitialized())
		return common::env::error.set("Uninitialized value in expression");

	auto object = get_object_()->remove_reference();
	if (is_constant())
		common::env::runtime.operand.constant_objects.push_back(object);

	return object->evaluate(info);
}

cscript::object::generic *cscript::object::ref::evaluate(const unary_info &info){
	if (is_uninitialized()){
		if (info.left && info.id == lexer::operator_id::bitwise_and)
			return common::env::temp_storage.add(common::env::create_pointer(memory_value_, is_constant()));
		return common::env::error.set("Uninitialized value in expression");
	}

	auto object = get_object_()->remove_reference();
	if (is_constant())
		common::env::runtime.operand.constant_objects.push_back(object);

	return object->evaluate(info);
}

bool cscript::object::ref::to_bool(){
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return false;
	}

	return get_object_()->to_bool();
}

std::string cscript::object::ref::to_string(){
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return "";
	}

	return get_object_()->to_string();
}

std::string cscript::object::ref::echo(){
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return "";
	}

	return get_object_()->echo();
}

bool cscript::object::ref::is_reference(){
	return true;
}

bool cscript::object::ref::is_constant(){
	return (is_constant_ || basic::is_constant());
}

cscript::object::generic *cscript::object::ref::get_object_(){
	return common::env::address_space.get_entry(get_value_()).info.object;
}

cscript::object::ref::value_type cscript::object::ref::get_value_(){
	return common::env::address_space.convert<value_type>(memory_value_, get_type());
}
