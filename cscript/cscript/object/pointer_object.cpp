#include "pointer_object.h"
#include "byte_object.h"

cscript::object::pointer::pointer()
	: basic(common::env::address_space.add<value_type>()){
	auto &memory_entry = get_memory();
	memory_entry.info.type = common::env::nullptr_type;
	memory::pool::write_unchecked(memory_entry.base, static_cast<value_type>(0));
	CSCRIPT_SET(memory_entry.attributes, memory::virtual_address::attribute::byte_aligned);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::pointer::pointer(const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>()){
	get_memory().info.type = std::make_shared<type::pointer>(type);
}

cscript::object::pointer::pointer(bool){}

cscript::object::pointer::pointer(value_type value)
	: basic(common::env::address_space.add<value_type>()){
	auto &memory_entry = get_memory();
	auto &entry = common::env::address_space.get_entry(value);
	if (entry.value == 0ull)
		CSCRIPT_SET(memory_entry.attributes, memory::virtual_address::attribute::byte_aligned);

	memory_entry.info.type = std::make_shared<type::pointer>(entry.info.type);
	memory::pool::write_unchecked(memory_entry.base, value);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::pointer::pointer(value_type value, type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>()){
	auto &memory_entry = get_memory();
	memory_entry.info.type = std::make_shared<type::pointer>(type);
	memory::pool::write_unchecked(memory_entry.base, value);

	auto &entry = common::env::address_space.get_entry(value);
	if (entry.value == 0ull || (!entry.info.type->is_any() && !entry.info.type->is_same(type.get())))
		CSCRIPT_SET(memory_entry.attributes, memory::virtual_address::attribute::byte_aligned);

	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::pointer::pointer(memory::virtual_address::base_type base, const type::generic::ptr_type type)
	: basic(common::env::address_space.add<value_type>(false)){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory_entry.base = base;
}

cscript::object::pointer::~pointer(){}

cscript::object::generic *cscript::object::pointer::clone(){
	return common::env::temp_storage.add(std::make_shared<pointer>(get_value_()));
}

cscript::object::generic *cscript::object::pointer::cast(const type::generic *type){
	switch (type->get_id()){
	case type::id::byte:
		return common::env::temp_storage.add(std::make_shared<primitive::byte>(
			get_value<primitive::byte::value_type>()));
	case type::id::char_:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::char_type, get_value<char>()));
	case type::id::uchar:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::uchar_type, get_value<unsigned char>()));
	case type::id::short_:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::short_type, get_value<short>()));
	case type::id::ushort:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::ushort_type, get_value<unsigned short>()));
	case type::id::int_:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::int_type, get_value<int>()));
	case type::id::uint:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::uint_type, get_value<unsigned int>()));
	case type::id::long_:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::long_type, get_value<long>()));
	case type::id::ulong:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::ulong_type, get_value<unsigned long>()));
	case type::id::llong:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::llong_type, get_value<long long>()));
	case type::id::ullong:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(
			common::env::ullong_type, get_value<unsigned long long>()));
	default:
		break;
	}

	if (get_memory().info.type->has_conversion(type))
		return clone();

	if (type->is_pointer()){
		auto non_pointer = type->remove_pointer();
		if (non_pointer->is_any() || non_pointer->is_auto())
			return clone();

		if (non_pointer->is_same(common::env::byte_type.get()))
			return common::env::temp_storage.add(std::make_shared<pointer>(get_value_(), common::env::byte_type));
	}

	return nullptr;
}

cscript::object::generic *cscript::object::pointer::evaluate(const binary_info &info){
	if (is_uninitialized())
		return basic::evaluate(info);

	auto operand = common::env::get_object_operand();
	if (operand == nullptr || (operand = operand->remove_reference()) == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	auto operand_type = operand->get_type();
	if (operand_type == nullptr)
		return basic::evaluate(info);

	auto &memory_entry = get_memory();
	if (!memory_entry.info.type->is_nullptr() && !operand_type->is_nullptr() && !operand_type->is_same(memory_entry.info.type.get())){
		if (operand_type->is_integral()){
			auto value = operand->query<primitive::numeric>()->get_value<int>();
			auto type_size = memory_entry.info.type->remove_pointer()->get_size();

			switch (info.id){
			case lexer::operator_id::compound_plus:
				if (!is_lvalue() || is_constant())
					return common::env::error.set("Operator does not take specified operands");

				common::env::address_space.write(memory_value_, memory_entry.info.type, get_value_() + (value * type_size));
				return this;
			case lexer::operator_id::compound_minus:
				if (!is_lvalue() || is_constant())
					return common::env::error.set("Operator does not take specified operands");

				common::env::address_space.write(memory_value_, memory_entry.info.type, get_value_() - (value * type_size));
				return this;
			case lexer::operator_id::plus:
				return common::env::temp_storage.add(std::make_shared<pointer>(get_value_() + (value * type_size)));
			case lexer::operator_id::minus:
				return common::env::temp_storage.add(std::make_shared<pointer>(get_value_() - (value * type_size)));
			case lexer::operator_id::member_pointer_access:
			{
				auto &entry = common::env::address_space.get_entry(value);
				if (entry.info.object == nullptr)
					return common::env::error.set("Operator does not take specified operands");

				return entry.info.object->evaluate(binary_info{ lexer::operator_id::member_access });
			}
			default:
				break;
			}
		}

		return basic::evaluate(info);
	}

	switch (info.id){
	case lexer::operator_id::less:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(
			(get_value_() < operand->query<pointer>()->get_value_()) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::less_or_equal:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(
			(get_value_() <= operand->query<pointer>()->get_value_()) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::equality:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(
			(get_value_() == operand->query<pointer>()->get_value_()) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::inverse_equality:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(
			(get_value_() != operand->query<pointer>()->get_value_()) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::more_or_equal:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(
			(get_value_() >= operand->query<pointer>()->get_value_()) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::more:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(
			(get_value_() > operand->query<pointer>()->get_value_()) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	default:
		break;
	}

	return basic::evaluate(info);
}

cscript::object::generic *cscript::object::pointer::evaluate(const unary_info &info){
	if (info.left){
		switch (info.id){
		case lexer::operator_id::times://Dereference
		{
			if (is_null())
				return common::env::error.set("Operator does not take specified operand");

			if (!CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::byte_aligned))
				return common::env::address_space.get_entry(get_value_()).info.object;

			auto base_type = get_type()->query<type::pointer>()->get_value();
			return common::env::temp_storage.add(base_type->create_ref(get_value_(), is_constant_target(), base_type));
		}
		case lexer::operator_id::decrement:
			if (is_constant() || !is_lvalue())
				return common::env::error.set("Operator does not take specified operand");
			return offset_(false);
		case lexer::operator_id::increment:
			if (is_constant() || !is_lvalue())
				return common::env::error.set("Operator does not take specified operand");
			return offset_(true);
		default:
			break;
		}
	}
	else{//Right unary
		switch (info.id){
		case lexer::operator_id::decrement:
		{
			if (is_constant() || !is_lvalue())
				return common::env::error.set("Operator does not take specified operand");

			auto old = clone();
			offset_(false);

			return old;
		}
		case lexer::operator_id::increment:
		{
			if (is_constant() || !is_lvalue())
				return common::env::error.set("Operator does not take specified operand");

			auto old = clone();
			return offset_(true);

			return old;
		}
		default:
			break;
		}
	}

	return basic::evaluate(info);
}

bool cscript::object::pointer::to_bool(){
	return false;
}

std::string cscript::object::pointer::echo(){
	if (is_null())
		return "nullptr";

	auto target = evaluate(unary_info{ true, lexer::operator_id::times });
	if (common::env::error.has())
		return "";

	if (target == nullptr){
		common::env::error.set("Cannot echo object");
		return "";
	}

	auto echo_value = target->echo();
	if (common::env::error.has())
		return "";

	return ("<" + common::env::to_hex(get_value_()) + ": " + echo_value + ">");
}

bool cscript::object::pointer::is_constant_target(){
	return CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::const_target);
}

bool cscript::object::pointer::is_null(){
	return (get_value_() == 0u);
}

cscript::object::generic *cscript::object::pointer::post_assignment_(generic &operand){
	if (CSCRIPT_IS(operand.get_memory().attributes, memory::virtual_address::attribute::byte_aligned))
		CSCRIPT_SET(get_memory().attributes, memory::virtual_address::attribute::byte_aligned);
	else
		CSCRIPT_REMOVE(get_memory().attributes, memory::virtual_address::attribute::byte_aligned);

	return this;
}

cscript::object::generic *cscript::object::pointer::offset_(bool increment){
	if (is_uninitialized())
		return common::env::error.set("Uninitialized object in expression");

	auto type = get_type();
	auto type_size = type->remove_pointer()->get_size();

	CSCRIPT_SET(get_memory().attributes, memory::virtual_address::attribute::byte_aligned);
	if (increment)
		common::env::address_space.write(memory_value_, type, get_value_() + type_size);
	else//Decrement
		common::env::address_space.write(memory_value_, type, get_value_() - type_size);
	
	return this;
}

cscript::object::pointer::value_type cscript::object::pointer::get_value_(){
	return common::env::address_space.convert<value_type>(memory_value_, get_type());
}

cscript::object::pointer_ref::pointer_ref(memory::virtual_address::value_type memory_value,
	const type::generic::ptr_type type, bool is_constant)
	: pointer(false), memory_(common::env::address_space.get_bound_entry(memory_value)){
	memory_value_ = memory_value;
	memory_.info.type = type;
	if (is_constant)
		CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::constant);
}

cscript::object::pointer_ref::~pointer_ref(){}

cscript::memory::virtual_address::entry &cscript::object::pointer_ref::get_memory(){
	return memory_;
}

bool cscript::object::pointer_ref::is_lvalue(){
	return true;
}
