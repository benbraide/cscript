#include "pointer_object.h"

cscript::object::pointer::pointer(const value_type &value)
	: pointer(common::env::address_space, value){}

cscript::object::pointer::pointer(memory::virtual_address &address_space, const value_type &value)
	: basic(address_space.add<value_type>()){
	auto &entry = value.address->get_entry(value);
	if (value.offset != entry.offset || value.value != entry.value){
		memory_.type = std::make_shared<type::pointer>(common::env::byte_type);
		CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::byte_aligned);
	}
	else//No offset
		memory_.type = std::make_shared<type::pointer>(entry.type);

	memory::pool::write_ref_unchecked(memory_.base, value);
	CSCRIPT_REMOVE(memory_.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::pointer::pointer(memory::virtual_address &address_space, const value_type &value, type::generic::ptr_type type)
	: basic(address_space.add<value_type>()){
	memory_.type = std::make_shared<type::pointer>(type);
	memory::pool::write_ref_unchecked(memory_.base, value);

	CSCRIPT_REMOVE(memory_.attributes, memory::virtual_address::attribute::uninitialized);
	CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::byte_aligned);
}

cscript::object::pointer::~pointer(){}

cscript::object::generic *cscript::object::pointer::clone(){
	return common::env::temp_storage.add(std::make_shared<pointer>(common::env::temp_address_space, get_value_()));
}

cscript::object::generic *cscript::object::pointer::cast(const type::generic *type){
	switch (type->get_id()){
	case type::id::char_:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::char_type, get_value<char>()));
	case type::id::uchar:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::uchar_type, get_value<unsigned char>()));
	case type::id::short_:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::short_type, get_value<short>()));
	case type::id::ushort:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::ushort_type, get_value<unsigned short>()));
	case type::id::int_:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::int_type, get_value<int>()));
	case type::id::uint:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::uint_type, get_value<unsigned int>()));
	case type::id::long_:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::long_type, get_value<long>()));
	case type::id::ulong:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::ulong_type, get_value<unsigned long>()));
	case type::id::llong:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::llong_type, get_value<long long>()));
	case type::id::ullong:
		return common::env::temp_storage.add(std::make_shared<primitive::numeric>(common::env::temp_address_space,
			common::env::ullong_type, get_value<unsigned long long>()));
	default:
		break;
	}

	if (memory_.type->has_conversion(type))
		return clone();

	if (type->is_pointer()){
		auto non_pointer = type->remove_pointer();
		if (non_pointer->is_any() || non_pointer->is_auto())
			return clone();

		if (non_pointer->is_same(common::env::byte_type.get()))
			return common::env::temp_storage.add(std::make_shared<pointer>(common::env::temp_address_space, get_value_(), common::env::byte_type));
	}

	return nullptr;
}

cscript::object::generic *cscript::object::pointer::evaluate(const binary_info &info){
	auto operand = common::env::get_object_operand();
	if (operand == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	auto operand_type = operand->get_type();
	if (operand_type == nullptr)
		return basic::evaluate(info);

	if (!operand_type->is_same(memory_.type.get())){
		if (operand_type->is_integral()){
			auto &value = get_value_();
			switch (info.id){
			case lexer::operator_id::compound_plus:
				if (!is_lvalue() || is_constant())
					return common::env::error.set("Operator does not take specified operands");

				value.value += (operand->query<primitive::numeric>()->get_value<int>() *
					memory_.type->remove_pointer()->get_size());

				return this;
			case lexer::operator_id::compound_minus:
				if (!is_lvalue() || is_constant())
					return common::env::error.set("Operator does not take specified operands");

				value.value -= (operand->query<primitive::numeric>()->get_value<int>() *
					memory_.type->remove_pointer()->get_size());

				return this;
			case lexer::operator_id::plus:
				return common::env::temp_storage.add(std::make_shared<pointer>(common::env::temp_address_space,
					value_type{ value.address, value.value + (operand->query<primitive::numeric>()->get_value<int>() *
						memory_.type->remove_pointer()->get_size()), value.offset }));
			case lexer::operator_id::minus:
				return common::env::temp_storage.add(std::make_shared<pointer>(common::env::temp_address_space,
					value_type{ value.address, value.value - (operand->query<primitive::numeric>()->get_value<int>() *
						memory_.type->remove_pointer()->get_size()), value.offset }));
			case lexer::operator_id::member_pointer_access:
			{
				auto &entry = value.address->get_entry(value);
				if (value.address->is_none(entry))
					return common::env::error.set("Operator does not take specified operands");

				return entry.object->evaluate(binary_info{ lexer::operator_id::member_access });
			}
			default:
				break;
			}
		}

		return basic::evaluate(info);
	}

	switch (info.id){
	case lexer::operator_id::less:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(common::env::temp_address_space,
			(memory::virtual_address::compare(get_value_(), operand->query<pointer>()->get_value_()) < 0) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::less_or_equal:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(common::env::temp_address_space,
			(memory::virtual_address::compare(get_value_(), operand->query<pointer>()->get_value_()) <= 0) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::equality:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(common::env::temp_address_space,
			(memory::virtual_address::compare(get_value_(), operand->query<pointer>()->get_value_()) == 0) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::inverse_equality:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(common::env::temp_address_space,
			(memory::virtual_address::compare(get_value_(), operand->query<pointer>()->get_value_()) != 0) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::more_or_equal:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(common::env::temp_address_space,
			(memory::virtual_address::compare(get_value_(), operand->query<pointer>()->get_value_()) >= 0) ?
			type::boolean_value_type::true_ : type::boolean_value_type::false_));
	case lexer::operator_id::more:
		return common::env::temp_storage.add(std::make_shared<primitive::boolean>(common::env::temp_address_space,
			(memory::virtual_address::compare(get_value_(), operand->query<pointer>()->get_value_()) > 0) ?
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
			break;
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

cscript::object::generic *cscript::object::pointer::offset_(bool increment){
	if (is_uninitialized())
		return common::env::error.set("Uninitialized object in expression");

	auto &value = get_value_();
	if (increment){
		return common::env::temp_storage.add(std::make_shared<pointer>(common::env::temp_address_space,
			value_type{ value.address, value.value + memory_.type->remove_pointer()->get_size(), value.offset }));
	}
	
	return common::env::temp_storage.add(std::make_shared<pointer>(common::env::temp_address_space,
		value_type{ value.address, value.value - memory_.type->remove_pointer()->get_size(), value.offset }));
}

cscript::object::pointer::value_type &cscript::object::pointer::get_value_(){
	return memory::pool::convert_unchecked<value_type>(memory_.base);
}
