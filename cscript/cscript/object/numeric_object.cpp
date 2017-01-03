#include "pointer_object.h"

cscript::object::primitive::numeric::numeric(const type::generic::ptr_type type)
	: numeric(common::env::address_space, type){}

cscript::object::primitive::numeric::numeric(memory::virtual_address &address_space, const type::generic::ptr_type type)
	: basic(address_space.add(type->get_size())){
	memory_.type = type;
}

cscript::object::primitive::numeric::numeric(memory::virtual_address::entry &parent, const type::generic::ptr_type type)
	: basic(parent.address->add(parent, type->get_size())){
	memory_.type = type;
}

cscript::object::primitive::numeric::~numeric(){}

cscript::object::generic *cscript::object::primitive::numeric::clone(){
	auto value = std::make_shared<numeric>(common::env::temp_address_space, memory_.type);

	common::env::temp_storage.add(value);
	memory::virtual_address::copy(memory_, value->get_memory());

	return value.get();
}

cscript::object::generic *cscript::object::primitive::numeric::cast(const type::generic *type){
	switch (type->get_id()){
	case type::id::char_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::char_type, get_value<char>()));
	case type::id::uchar:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::uchar_type, get_value<unsigned char>()));
	case type::id::short_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::short_type, get_value<short>()));
	case type::id::ushort:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::ushort_type, get_value<unsigned short>()));
	case type::id::int_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::int_type, get_value<int>()));
	case type::id::uint:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::uint_type, get_value<unsigned int>()));
	case type::id::long_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::long_type, get_value<long>()));
	case type::id::ulong:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::ulong_type, get_value<unsigned long>()));
	case type::id::llong:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::llong_type, get_value<long long>()));
	case type::id::ullong:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::ullong_type, get_value<unsigned long long>()));
	case type::id::float_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::float_type, get_value<float>()));
	case type::id::double_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::double_type, get_value<double>()));
	case type::id::ldouble:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::temp_address_space,
			common::env::ldouble_type, get_value<long double>()));
	default:
		break;
	}

	if (type->is_pointer() && get_type()->is_integral()){
		auto value = get_value<memory::virtual_address::value_type>();
		switch (type->remove_pointer()->get_id()){
		case type::id::any:
		case type::id::byte:
			return common::env::temp_storage.add(std::make_shared<pointer>(pointer::value_type{
				&common::env::address_space, value, 0 }));
		default:
			break;
		}

		auto &entry = common::env::address_space.get_entry(value);
		if (common::env::address_space.is_none(entry) || entry.type == nullptr || !entry.type->is_same(type) ||
			common::env::address_space.convert_info(pointer::value_type{ nullptr, entry.value, entry.offset }) != value){
			return common::env::temp_storage.add(std::make_shared<pointer>(pointer::value_type{
				&common::env::address_space, 0, 0 }));//nullptr
		}

		return common::env::temp_storage.add(std::make_shared<pointer>(pointer::value_type{
			entry.address, entry.value, entry.offset }));
	}

	return memory_.type->has_conversion(type) ? clone() : basic::cast(type);
}

cscript::object::generic *cscript::object::primitive::numeric::evaluate(const binary_info &info){
	auto operand = common::env::get_object_operand();
	if (operand == nullptr || (operand = operand->remove_reference()) == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	if (info.id == lexer::operator_id::assignment){
		if (!is_lvalue() || (is_constant() && !is_uninitialized()))
			return common::env::error.set("Operator does not take specified operands");

		auto type = get_type();
		auto value = operand->ref_cast(type.get());
		if (value == nullptr && (value = operand->cast(type.get())) == nullptr)
			return common::env::error.set("Cannot assign value into object");

		CSCRIPT_REMOVE(memory_.attributes, memory::virtual_address::attribute::uninitialized);
		if (value->query<numeric>()->is_nan())
			CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::is_nan);
		else//Copy value
			memory::virtual_address::copy(memory_, value->get_memory());

		return this;
	}

	auto operand_type = operand->get_type();
	if (!operand_type->is_numeric())
		return basic::evaluate(info);

	auto bully = (memory_.type->get_bully(operand_type.get()) == memory_.type.get()) ? memory_.type : operand_type;
	auto numeric_operand = operand->query<numeric>();

	switch (info.id){
	case lexer::operator_id::compound_plus:
		return add_(bully, *numeric_operand, true);
	case lexer::operator_id::compound_minus:
		return subtract_(bully, *numeric_operand, true);
	case lexer::operator_id::compound_times:
		return multiply_(bully, *numeric_operand, true);
	case lexer::operator_id::compound_divide:
		return divide_(bully, *numeric_operand, true);
	case lexer::operator_id::compound_modulus:
		return modulus_(bully, *numeric_operand, true);
	case lexer::operator_id::compound_left_shift:
		return left_shift_(bully, *numeric_operand, true);
	case lexer::operator_id::compound_right_shift:
		return right_shift_(bully, *numeric_operand, true);
	case lexer::operator_id::compound_bitwise_and:
		return bitwise_and_(bully, *numeric_operand, true);
	case lexer::operator_id::compound_bitwise_or:
		return bitwise_or_(bully, *numeric_operand, true);
	case lexer::operator_id::compound_bitwise_xor:
		return bitwise_xor_(bully, *numeric_operand, true);
	case lexer::operator_id::plus:
		return add_(bully, *numeric_operand, false);
	case lexer::operator_id::minus:
		return subtract_(bully, *numeric_operand, false);
	case lexer::operator_id::times:
		return multiply_(bully, *numeric_operand, false);
	case lexer::operator_id::divide:
		return divide_(bully, *numeric_operand, false);
	case lexer::operator_id::modulus:
		return modulus_(bully, *numeric_operand, false);
	case lexer::operator_id::left_shift:
		return left_shift_(bully, *numeric_operand, false);
	case lexer::operator_id::right_shift:
		return right_shift_(bully, *numeric_operand, false);
	case lexer::operator_id::bitwise_and:
		return bitwise_and_(bully, *numeric_operand, false);
	case lexer::operator_id::bitwise_or:
		return bitwise_or_(bully, *numeric_operand, false);
	case lexer::operator_id::bitwise_xor:
		return bitwise_xor_(bully, *numeric_operand, false);
	case lexer::operator_id::less:
		return create_boolean_((!is_nan() && !numeric_operand->is_nan() && compare_(bully, *numeric_operand) < 0) ?
			boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::less_or_equal:
		return create_boolean_(!is_nan() && !numeric_operand->is_nan() && (compare_(bully, *numeric_operand) <= 0) ?
			boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::equality:
		return create_boolean_((compare_(bully, *numeric_operand) == 0) ? boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::inverse_equality:
		return create_boolean_((compare_(bully, *numeric_operand) != 0) ? boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::more_or_equal:
		return create_boolean_(!is_nan() && !numeric_operand->is_nan() && (compare_(bully, *numeric_operand) >= 0) ?
			boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::more:
		return create_boolean_(!is_nan() && !numeric_operand->is_nan() && (compare_(bully, *numeric_operand) > 0) ?
			boolean::value_type::true_ : boolean::value_type::false_);
	default:
		break;
	}

	return basic::evaluate(info);
}

cscript::object::generic *cscript::object::primitive::numeric::evaluate(const unary_info &info){
	if (info.left){
		switch (info.id){
		case lexer::operator_id::plus:
			return plus_();
		case lexer::operator_id::minus:
			return minus_();
		case lexer::operator_id::bitwise_inverse:
			return bitwise_inverse_();
		case lexer::operator_id::decrement:
			common::env::object_operand = common::env::one.get();
			return subtract_(nullptr, *common::env::object_operand->query<numeric>(), true);
		case lexer::operator_id::increment:
			common::env::object_operand = common::env::one.get();
			return add_(nullptr, *common::env::object_operand->query<numeric>(), true);
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
			common::env::object_operand = common::env::one.get();
			subtract_(nullptr, *common::env::object_operand->query<numeric>(), true);

			return old;
		}
		case lexer::operator_id::increment:
		{
			if (is_constant() || !is_lvalue())
				return common::env::error.set("Operator does not take specified operand");

			auto old = clone();
			common::env::object_operand = common::env::one.get();
			add_(nullptr, *common::env::object_operand->query<numeric>(), true);

			return old;
		}
		default:
			break;
		}
	}

	return basic::evaluate(info);
}

std::string cscript::object::primitive::numeric::to_string(){
	return to_string_(false);
}

std::string cscript::object::primitive::numeric::echo(){
	return to_string_(true);
}

bool cscript::object::primitive::numeric::is_nan(){
	return CSCRIPT_IS(memory_.attributes, memory::virtual_address::attribute::is_nan);
}

cscript::object::generic::ptr_type cscript::object::primitive::numeric::clone_(const type::generic::ptr_type type, bool is_nan){
	auto value = std::make_shared<numeric>(common::env::temp_address_space, type);
	if (is_nan)
		CSCRIPT_SET(value->get_memory().attributes, memory::virtual_address::attribute::is_nan);

	return value;
}

cscript::object::generic *cscript::object::primitive::numeric::create_boolean_(boolean::value_type value){
	if (common::env::error.has())
		return nullptr;

	return common::env::temp_storage.add(std::make_shared<boolean>(common::env::temp_address_space, value));
}

cscript::object::generic *cscript::object::primitive::numeric::plus_(){
	if (is_uninitialized())
		return common::env::error.set("Uninitialized object in expression");

	auto op = get_op_();
	auto destination = common::env::temp_storage.add(clone_(memory_.type, is_nan()));

	CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
	if (!is_nan())
		op->plus(destination->get_memory().base, memory_.base);

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::minus_(){
	if (memory_.type->is_unsigned_integral())
		return common::env::error.set("Operator does not take specified operand");

	if (is_uninitialized())
		return common::env::error.set("Uninitialized object in expression");

	auto op = get_op_();
	auto destination = common::env::temp_storage.add(clone_(memory_.type, is_nan()));

	CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
	if (!is_nan())
		op->minus(destination->get_memory().base, memory_.base);

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::bitwise_inverse_(){
	if (!memory_.type->is_integral())
		return common::env::error.set("Operator does not take specified operand");

	if (is_uninitialized())
		return common::env::error.set("Uninitialized object in expression");

	auto op = get_op_();
	auto destination = common::env::temp_storage.add(clone_(memory_.type, is_nan()));

	CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
	if (!is_nan())
		op->bitwise_inverse(destination->get_memory().base, memory_.base);

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::add_(type::generic::ptr_type bully, numeric &operand, bool assign){
	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan())
			op->add(destination->get_memory().base, left, right);
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::subtract_(type::generic::ptr_type bully, numeric &operand, bool assign){
	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan())
			op->subtract(destination->get_memory().base, left, right);
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::multiply_(type::generic::ptr_type bully, numeric &operand, bool assign){
	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan())
			op->multiply(destination->get_memory().base, left, right);
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::divide_(type::generic::ptr_type bully, numeric &operand, bool assign){
	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan()){
			if (compare(0) == 0)//Division by zero
				CSCRIPT_SET(destination->get_memory().attributes, memory::virtual_address::attribute::is_nan);
			else
				op->divide(destination->get_memory().base, left, right);
		}
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::modulus_(type::generic::ptr_type bully, numeric &operand, bool assign){
	if (!memory_.type->is_integral() || !operand.get_memory().type->is_integral())
		return common::env::error.set("Operator does not take specified operand");

	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan()){
			if (compare(0) == 0)//Division by zero
				CSCRIPT_SET(destination->get_memory().attributes, memory::virtual_address::attribute::is_nan);
			else
				op->modulus(destination->get_memory().base, left, right);
		}
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::left_shift_(type::generic::ptr_type bully, numeric &operand, bool assign){
	if (!memory_.type->is_integral() || !operand.get_memory().type->is_integral())
		return common::env::error.set("Operator does not take specified operand");

	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan())
			op->left_shift(destination->get_memory().base, left, right);
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::right_shift_(type::generic::ptr_type bully, numeric &operand, bool assign){
	if (!memory_.type->is_integral() || !operand.get_memory().type->is_integral())
		return common::env::error.set("Operator does not take specified operand");

	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan())
			op->right_shift(destination->get_memory().base, left, right);
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::bitwise_and_(type::generic::ptr_type bully, numeric &operand, bool assign){
	if (!memory_.type->is_integral() || !operand.get_memory().type->is_integral())
		return common::env::error.set("Operator does not take specified operand");

	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan())
			op->bitwise_and(destination->get_memory().base, left, right);
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::bitwise_or_(type::generic::ptr_type bully, numeric &operand, bool assign){
	if (!memory_.type->is_integral() || !operand.get_memory().type->is_integral())
		return common::env::error.set("Operator does not take specified operand");

	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan())
			op->bitwise_or(destination->get_memory().base, left, right);
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::bitwise_xor_(type::generic::ptr_type bully, numeric &operand, bool assign){
	if (!memory_.type->is_integral() || !operand.get_memory().type->is_integral())
		return common::env::error.set("Operator does not take specified operand");

	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan())
			op->bitwise_xor(destination->get_memory().base, left, right);
	}

	return destination;
}

int cscript::object::primitive::numeric::compare_(type::generic::ptr_type bully, numeric &operand){
	if (is_uninitialized() || operand.is_uninitialized()){
		common::env::error.set("Uninitialized object in expression");
		return -2;
	}

	if (is_nan())
		return operand.is_nan() ? 0 : -1;

	if (operand.is_nan())
		return is_nan() ? 0 : -1;

	memory::pool::base_type left, right;
	block_operator_type op;
	if (bully.get() == memory_.type.get()){
		left = memory_.base;
		op = cast_value_(bully->get_id(), operand);
		right = common::env::static_block1.get_base();
	}
	else{//Cast this
		left = operand.get_memory().base;
		op = cast_value_(bully->get_id(), *this);
		right = common::env::static_block1.get_base();
	}

	return op->compare(left, right);
}

cscript::object::primitive::numeric::block_operator_type cscript::object::primitive::numeric::prepare_(type::generic::ptr_type bully,
	numeric &operand, bool assign, generic *&destination, memory::pool::base_type &left, memory::pool::base_type &right){
	if (is_uninitialized() || operand.is_uninitialized()){
		destination = common::env::error.set("Uninitialized object in expression");
		return nullptr;
	}

	if (assign){
		if (is_constant() || !is_lvalue()){
			destination = common::env::error.set("Operator does not take specified operand");
			return nullptr;
		}

		bully = memory_.type;
		destination = this;
	}
	else//Create temp
		destination = common::env::temp_storage.add(clone_(bully, is_nan() || operand.is_nan()));

	block_operator_type op;
	if (bully.get() == memory_.type.get()){
		left = memory_.base;
		op = cast_value_(bully->get_id(), operand);
		right = common::env::static_block1.get_base();
	}
	else{//Cast this
		right = operand.get_memory().base;
		op = cast_value_(bully->get_id(), *this);
		left = common::env::static_block1.get_base();
	}

	return op;
}

cscript::object::primitive::numeric::block_operator_type cscript::object::primitive::numeric::get_op_(){
	switch (memory_.type->get_id()){
	case type::id::char_:
		return std::make_shared<cscript::memory::typed_block_operator<char> >();
	case type::id::uchar:
		return std::make_shared<cscript::memory::typed_block_operator<unsigned char> >();
	case type::id::short_:
		return std::make_shared<cscript::memory::typed_block_operator<short> >();
	case type::id::ushort:
		return std::make_shared<cscript::memory::typed_block_operator<unsigned short> >();
	case type::id::int_:
		return std::make_shared<cscript::memory::typed_block_operator<int> >();
	case type::id::uint:
		return std::make_shared<cscript::memory::typed_block_operator<unsigned int> >();
	case type::id::long_:
		return std::make_shared<cscript::memory::typed_block_operator<long> >();
	case type::id::ulong:
		return std::make_shared<cscript::memory::typed_block_operator<unsigned long> >();
	case type::id::llong:
		return std::make_shared<cscript::memory::typed_block_operator<long long> >();
	case type::id::ullong:
		return std::make_shared<cscript::memory::typed_block_operator<unsigned long long> >();
	case type::id::float_:
		return std::make_shared<cscript::memory::typed_block_operator<float> >();
	case type::id::double_:
		return std::make_shared<cscript::memory::typed_block_operator<double> >();
	case type::id::ldouble:
		return std::make_shared<cscript::memory::typed_block_operator<long double> >();
	default:
		break;
	}

	return nullptr;
}

std::string cscript::object::primitive::numeric::to_string_(bool echo){
	switch (get_memory().type->get_id()){
	case type::id::char_:
		return echo ? ("'" + std::string(1, get_value<char>()) + "'") : std::string(1, get_value<char>());
	case type::id::uchar:
		return echo ? ("'" + std::string(1, get_value<char>()) + "'u") : std::string(1, get_value<char>());
	case type::id::short_:
		return std::to_string(get_value<short>());
	case type::id::ushort:
		return echo ? (std::to_string(get_value<short>()) + "u") : std::to_string(get_value<short>());
	case type::id::int_:
		return std::to_string(get_value<int>());
	case type::id::uint:
		return echo ? (std::to_string(get_value<unsigned int>()) + "u") : std::to_string(get_value<unsigned int>());
	case type::id::long_:
		return echo ? (std::to_string(get_value<long>()) + "l") : std::to_string(get_value<long>());
	case type::id::ulong:
		return echo ? (std::to_string(get_value<unsigned long>()) + "ul") : std::to_string(get_value<unsigned long>());
	case type::id::llong:
		return echo ? (std::to_string(get_value<long long>()) + "ll") : std::to_string(get_value<long long>());
	case type::id::ullong:
		return echo ? (std::to_string(get_value<unsigned long long>()) + "ull") : std::to_string(get_value<unsigned long long>());
	case type::id::float_:
		return echo ? (std::to_string(get_value<float>()) + "f") : std::to_string(get_value<float>());
	case type::id::double_:
		return std::to_string(get_value<double>());
	case type::id::ldouble:
		return echo ? (std::to_string(get_value<long double>()) + "l") : std::to_string(get_value<long double>());
	default:
		break;
	}

	return basic::to_string();
}

cscript::object::primitive::numeric::block_operator_type cscript::object::primitive::numeric::cast_value_(type::id id, numeric &source){
	switch (id){
	case type::id::char_:
		source.cast_value<char>();
		return std::make_shared<cscript::memory::typed_block_operator<char> >();
	case type::id::uchar:
		source.cast_value<unsigned char>();
		return std::make_shared<cscript::memory::typed_block_operator<unsigned char> >();
	case type::id::short_:
		source.cast_value<short>();
		return std::make_shared<cscript::memory::typed_block_operator<short> >();
	case type::id::ushort:
		source.cast_value<unsigned short>();
		return std::make_shared<cscript::memory::typed_block_operator<unsigned short> >();
	case type::id::int_:
		source.cast_value<int>();
		return std::make_shared<cscript::memory::typed_block_operator<int> >();
	case type::id::uint:
		source.cast_value<unsigned int>();
		return std::make_shared<cscript::memory::typed_block_operator<unsigned int> >();
	case type::id::long_:
		source.cast_value<long>();
		return std::make_shared<cscript::memory::typed_block_operator<long> >();
	case type::id::ulong:
		source.cast_value<unsigned long>();
		return std::make_shared<cscript::memory::typed_block_operator<unsigned long> >();
	case type::id::llong:
		source.cast_value<long long>();
		return std::make_shared<cscript::memory::typed_block_operator<long long> >();
	case type::id::ullong:
		source.cast_value<unsigned long long>();
		return std::make_shared<cscript::memory::typed_block_operator<unsigned long long> >();
	case type::id::float_:
		source.cast_value<float>();
		return std::make_shared<cscript::memory::typed_block_operator<float> >();
	case type::id::double_:
		source.cast_value<double>();
		return std::make_shared<cscript::memory::typed_block_operator<double> >();
	case type::id::ldouble:
		source.cast_value<long double>();
		return std::make_shared<cscript::memory::typed_block_operator<long double> >();
	default:
		break;
	}

	return nullptr;
}
