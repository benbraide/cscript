#include "type_object.h"
#include "pointer_object.h"

cscript::object::primitive::type_object::type_object()
	: basic(common::env::address_space.add<value_type>()){
	get_memory().info.type = common::env::type_object_type;
}

cscript::object::primitive::type_object::type_object(bool){}

cscript::object::primitive::type_object::type_object(type::generic::ptr_type value)
	: basic(common::env::address_space.add<value_type>()), value_(value){
	auto &memory_entry = get_memory();
	memory_entry.info.type = common::env::type_object_type;
	memory::pool::write_unchecked(memory_entry.base, reinterpret_cast<value_type>(value.get()));
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::primitive::type_object::type_object(value_type value)
	: basic(common::env::address_space.add<value_type>()){
	auto &memory_entry = get_memory();
	memory_entry.info.type = common::env::type_object_type;

	memory::pool::write_unchecked(memory_entry.base, value);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);

	auto type_object_value = (memory_entry.info.object == nullptr) ? nullptr : memory_entry.info.object->query<type_object>();
	value_ = ((type_object_value == nullptr) ? nullptr : type_object_value->get_type_value());
}

cscript::object::primitive::type_object::type_object(memory::virtual_address::base_type base)
	: basic(common::env::address_space.add<value_type>(false)){
	auto &memory_entry = get_memory();
	memory_entry.info.type = common::env::type_object_type;
	memory_entry.base = base;
}

cscript::object::primitive::type_object::~type_object(){}

cscript::object::generic *cscript::object::primitive::type_object::clone(){
	if (is_uninitialized())
		return nullptr;

	if (value_ == nullptr)
		return common::env::temp_storage.add(std::make_shared<type_object>(get_value_()));

	return common::env::temp_storage.add(std::make_shared<type_object>(value_));
}

cscript::object::generic *cscript::object::primitive::type_object::evaluate(const binary_info &info){
	if (is_uninitialized())
		return basic::evaluate(info);

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
	case lexer::operator_id::call:
		return operand->cast(get_value<type::generic *>());
	default:
		break;
	}

	return basic::evaluate(info);
}

cscript::object::generic *cscript::object::primitive::type_object::evaluate(const unary_info &info){
	if (is_uninitialized())
		return basic::evaluate(info);

	if (info.left && info.id == lexer::operator_id::sizeof_){
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::uint_type, (value_ == nullptr) ? get_type()->get_size() : value_->get_size()));
	}

	return basic::evaluate(info);
}

std::string cscript::object::primitive::type_object::echo(){
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return "";
	}

	return get_value<type::generic *>()->print();
}

cscript::type::generic::ptr_type cscript::object::primitive::type_object::get_type_value() const{
	return value_;
}

cscript::object::generic *cscript::object::primitive::type_object::post_assignment_(generic &operand){
	auto type_operand = operand.query<type_object>();
	value_ = ((type_operand == nullptr) ? nullptr : type_operand->value_);
	return this;
}

cscript::object::primitive::type_object::value_type cscript::object::primitive::type_object::get_value_(){
	return common::env::address_space.convert<value_type>(memory_value_, get_type());
}

cscript::object::primitive::boolean::value_type cscript::object::primitive::type_object::compare_(generic &operand, bool equality){
	auto type_operand = operand.query<type_object>();
	return (type_operand->get_value<type::generic *>()->is_same(get_value<type::generic *>()) == equality) ?
		boolean::value_type::true_ : boolean::value_type::false_;
}

cscript::object::primitive::type_object_ref::type_object_ref(memory::virtual_address::value_type memory_value, bool is_constant)
	: type_object(false), memory_(common::env::address_space.get_bound_entry(memory_value)){
	memory_value_ = memory_value;
	memory_.info.type = common::env::type_object_type;
	if (is_constant)
		CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::constant);

	auto &memory_entry = common::env::address_space.get_entry(memory_value);
	auto type_object_value = (memory_entry.info.object == nullptr) ? nullptr : memory_entry.info.object->query<type_object>();

	value_ = ((type_object_value == nullptr) ? nullptr : type_object_value->get_type_value());
}

cscript::object::primitive::type_object_ref::~type_object_ref(){}

cscript::memory::virtual_address::entry &cscript::object::primitive::type_object_ref::get_memory(){
	return memory_;
}

bool cscript::object::primitive::type_object_ref::is_lvalue(){
	return true;
}
