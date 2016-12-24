#include "numeric_object.h"

cscript::object::primitive::numeric::numeric(const type::generic::ptr_type type)
	: numeric(common::env::address_space, type){}

cscript::object::primitive::numeric::numeric(memory::virtual_address &address_space, const type::generic::ptr_type type)
	: basic(address_space.add(type->get_size())){
	memory_.type = type;
}

cscript::object::primitive::numeric::~numeric(){}

cscript::object::generic *cscript::object::primitive::numeric::clone(){
	auto value = std::make_shared<numeric>(memory_.type);

	common::env::temp_storage.add(value);
	memory::virtual_address::copy(memory_, value->get_memory());

	return value.get();
}

cscript::object::generic *cscript::object::primitive::numeric::cast(const type::generic *type){
	return memory_.type->has_conversion(type) ? clone() : basic::cast(type);
}

cscript::object::generic *cscript::object::primitive::numeric::evaluate(const binary_info &info){
	auto operand = common::env::get_object_operand();
	if (operand == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	auto operand_type = operand->get_type();
	if (!operand_type->is_numeric())
		return basic::evaluate(info);

	auto bully = (memory_.type->get_bully(operand_type.get()) == memory_.type.get()) ? memory_.type : operand_type;
	switch (info.id){
	case lexer::operator_id::compound_plus:
		return add_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::compound_minus:
		return subtract_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::compound_times:
		return multiply_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::compound_divide:
		return divide_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::compound_modulus:
		return modulus_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::compound_left_shift:
		return left_shift_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::compound_right_shift:
		return right_shift_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::compound_bitwise_and:
		return bitwise_and_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::compound_bitwise_or:
		return bitwise_or_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::compound_bitwise_xor:
		return bitwise_xor_(bully, *operand->query<numeric>(), true);
	case lexer::operator_id::plus:
		return add_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::minus:
		return subtract_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::times:
		return multiply_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::divide:
		return divide_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::modulus:
		return modulus_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::left_shift:
		return left_shift_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::right_shift:
		return right_shift_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::bitwise_and:
		return bitwise_and_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::bitwise_or:
		return bitwise_or_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::bitwise_xor:
		return bitwise_xor_(bully, *operand->query<numeric>(), false);
	case lexer::operator_id::less:
		return create_boolean_((compare_(bully, *operand->query<numeric>()) < 0) ? boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::less_or_equal:
		return create_boolean_((compare_(bully, *operand->query<numeric>()) <= 0) ? boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::equality:
		return create_boolean_((compare_(bully, *operand->query<numeric>()) == 0) ? boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::inverse_equality:
		return create_boolean_((compare_(bully, *operand->query<numeric>()) != 0) ? boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::more_or_equal:
		return create_boolean_((compare_(bully, *operand->query<numeric>()) >= 0) ? boolean::value_type::true_ : boolean::value_type::false_);
	case lexer::operator_id::more:
		return create_boolean_((compare_(bully, *operand->query<numeric>()) > 0) ? boolean::value_type::true_ : boolean::value_type::false_);
	default:
		break;
	}

	return basic::evaluate(info);
}

cscript::object::generic *cscript::object::primitive::numeric::evaluate(const unary_info &info){
	return basic::evaluate(info);
}

bool cscript::object::primitive::numeric::to_bool(){
	return (compare(0) != 0);
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

	return common::env::temp_storage.add(std::make_shared<boolean>(common::env::temp_address_space, common::env::bool_type, value));
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
		left = operand.get_memory().base;
		op = cast_value_(bully->get_id(), *this);
		right = common::env::static_block1.get_base();
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
