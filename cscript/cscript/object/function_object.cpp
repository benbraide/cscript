#include "function_object.h"
#include "pointer_object.h"

#include "../function/generic_function.h"

cscript::object::primitive::function_object::function_object(const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>()){
	get_memory().info.type = type;
}

cscript::object::primitive::function_object::function_object(bool){}

cscript::object::primitive::function_object::function_object(value_type value)
	: basic(common::env::address_space.add<value_type>()){
	auto &memory_entry = get_memory();
	if ((memory_entry.info.type = reinterpret_cast<function::generic *>(value)->get_type()) == nullptr)
		memory_entry.info.type = common::env::function_type;

	memory::pool::write_unchecked(memory_entry.base, value);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::primitive::function_object::function_object(memory::virtual_address::base_type base, const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>(false)){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory_entry.base = base;
}

cscript::object::primitive::function_object::~function_object(){}

cscript::object::generic *cscript::object::primitive::function_object::clone(){
	return is_uninitialized() ? nullptr : common::env::temp_storage.add(std::make_shared<function_object>(get_value_()));
}

cscript::object::generic *cscript::object::primitive::function_object::cast(const type::generic *type){
	if (type->is_function() && type->query<cscript::type::function>() == nullptr)
		return clone();

	auto matched = get_function_value()->get_matched(type);
	if (matched == nullptr)
		return nullptr;

	return common::env::temp_storage.add(std::make_shared<function_object>(reinterpret_cast<value_type>(matched)));
}

cscript::object::generic *cscript::object::primitive::function_object::ref_cast(const type::generic *type){
	return is_constant() ? nullptr : const_ref_cast(type);
}

cscript::object::generic *cscript::object::primitive::function_object::const_ref_cast(const type::generic *type){
	auto this_type = get_type();
	if (this_type->is_function() && this_type->query<cscript::type::function>() == nullptr)//Untyped function type
		return (type->is_function() && type->query<cscript::type::function>() == nullptr) ? this : nullptr;

	return basic::const_ref_cast(type);
}

cscript::object::generic *cscript::object::primitive::function_object::evaluate(const binary_info &info){
	if (is_uninitialized())
		return basic::evaluate(info);

	if (info.id == lexer::operator_id::call)
		return get_function_value()->call();

	auto operand = common::env::get_object_operand();
	if (operand == nullptr || (operand = operand->remove_reference()) == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	if (operand->get_type()->get_id() != type::id::type_)
		return basic::evaluate(info);

	switch (info.id){
	case lexer::operator_id::equality:
		return common::env::temp_storage.add(std::make_shared<boolean>(compare_(*operand, true)));
	case lexer::operator_id::inverse_equality:
		return common::env::temp_storage.add(std::make_shared<boolean>(compare_(*operand, false)));
	default:
		break;
	}

	return basic::evaluate(info);
}

std::string cscript::object::primitive::function_object::echo(){
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return "";
	}

	auto value = get_function_value()->print();
	if (value.empty()){
		common::env::error.set("");
		return "";
	}

	return value;
}

cscript::function::generic *cscript::object::primitive::function_object::get_function_value(){
	return reinterpret_cast<function::generic *>(get_value_());
}

cscript::object::primitive::function_object::value_type cscript::object::primitive::function_object::get_value_(){
	return common::env::address_space.convert<value_type>(memory_value_, get_type());
}

cscript::object::primitive::boolean::value_type cscript::object::primitive::function_object::compare_(generic &operand, bool equality){
	return ((operand.query<function_object>()->get_value_() == get_value_()) == equality) ?
		boolean::value_type::true_ : boolean::value_type::false_;
}

cscript::object::primitive::function_object_ref::function_object_ref(memory::virtual_address::value_type memory_value,
	const type::generic::ptr_type type, bool is_constant)
	: function_object(false), memory_(common::env::address_space.get_bound_entry(memory_value)){
	memory_value_ = memory_value;
	memory_.info.type = type;
	if (is_constant)
		CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::constant);
}

cscript::object::primitive::function_object_ref::~function_object_ref(){}

cscript::memory::virtual_address::entry &cscript::object::primitive::function_object_ref::get_memory(){
	return memory_;
}

bool cscript::object::primitive::function_object_ref::is_lvalue(){
	return true;
}
