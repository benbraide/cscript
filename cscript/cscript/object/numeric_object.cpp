#include "pointer_object.h"
#include "byte_object.h"

cscript::object::primitive::numeric::numeric()
	: basic(common::env::address_space.add<char>()){
	auto &memory_entry = get_memory();
	memory_entry.info.type = common::env::char_type;
	CSCRIPT_SET(memory_entry.attributes, memory::virtual_address::attribute::is_nan);
	CSCRIPT_REMOVE(memory_entry.attributes, memory::virtual_address::attribute::uninitialized);
}

cscript::object::primitive::numeric::numeric(bool){}

cscript::object::primitive::numeric::numeric(const type::generic::ptr_type type)
	: basic(common::env::address_space.add(type->get_size())){
	get_memory().info.type = type;
}

cscript::object::primitive::numeric::numeric(memory::virtual_address::base_type base, const type::generic::ptr_type type)
	: basic(common::env::address_space.add(type->get_size(), false)){
	auto &memory_entry = get_memory();
	memory_entry.info.type = type;
	memory_entry.base = base;
}

cscript::object::primitive::numeric::~numeric(){}

cscript::object::generic *cscript::object::primitive::numeric::clone(){
	if (is_uninitialized())
		return nullptr;

	auto value = std::make_shared<numeric>(get_memory().info.type);

	common::env::temp_storage.add(value);
	common::env::address_space.copy(value->get_memory_value(), memory_value_);
	CSCRIPT_REMOVE(value->get_memory().attributes, memory::virtual_address::attribute::uninitialized);

	return value.get();
}

cscript::object::generic *cscript::object::primitive::numeric::cast(const type::generic *type){
	if (is_uninitialized())
		return nullptr;

	switch (type->get_id()){
	case type::id::byte:
		return common::env::temp_storage.add(std::make_shared<byte>(get_value<byte::value_type>()));
	case type::id::char_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::char_type, get_value<char>()));
	case type::id::uchar:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::uchar_type, get_value<unsigned char>()));
	case type::id::short_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::short_type, get_value<short>()));
	case type::id::ushort:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::ushort_type, get_value<unsigned short>()));
	case type::id::int_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::int_type, get_value<int>()));
	case type::id::uint:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::uint_type, get_value<unsigned int>()));
	case type::id::long_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::long_type, get_value<long>()));
	case type::id::ulong:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::ulong_type, get_value<unsigned long>()));
	case type::id::llong:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::llong_type, get_value<long long>()));
	case type::id::ullong:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::ullong_type, get_value<unsigned long long>()));
	case type::id::float_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::float_type, get_value<float>()));
	case type::id::double_:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::double_type, get_value<double>()));
	case type::id::ldouble:
		return common::env::temp_storage.add(std::make_shared<numeric>(common::env::ldouble_type, get_value<long double>()));
	default:
		break;
	}

	if (type->is_pointer()){
		if (type->remove_pointer()->get_id() == cscript::type::id::char_)
			return common::env::temp_storage.add(common::env::create_string(to_string()));

		if (get_type()->is_integral()){
			auto value = get_value<memory::virtual_address::value_type>();
			auto base_type = type->query<cscript::type::pointer>()->get_value();

			return common::env::temp_storage.add(std::make_shared<pointer>(value, base_type));
		}
	}

	return get_type()->has_conversion(type) ? clone() : basic::cast(type);
}

cscript::object::generic *cscript::object::primitive::numeric::evaluate(const binary_info &info){
	if (is_uninitialized())
		return basic::evaluate(info);

	auto operand = common::env::get_object_operand();
	if (operand == nullptr || (operand = operand->remove_reference()) == nullptr)
		return common::env::error.set("Operator does not take specified operand");

	auto operand_type = operand->get_type();
	if (!operand_type->is_numeric()){
		auto pointer_operand = operand->query<pointer>();
		if (pointer_operand == nullptr || !pointer_operand->is_string())
			return basic::evaluate(info);

		switch (info.id){
		case lexer::operator_id::plus:
			return common::env::temp_storage.add(common::env::create_string(to_string() + operand->to_string()));
		case lexer::operator_id::less:
			return create_boolean_(boolean_value(to_string() < operand->to_string()));
		case lexer::operator_id::less_or_equal:
			return create_boolean_(boolean_value(to_string() <= operand->to_string()));
		case lexer::operator_id::equality:
			return create_boolean_(boolean_value(to_string() == operand->to_string()));
		case lexer::operator_id::inverse_equality:
			return create_boolean_(boolean_value(to_string() != operand->to_string()));
		case lexer::operator_id::more_or_equal:
			return create_boolean_(boolean_value(to_string() >= operand->to_string()));
		case lexer::operator_id::more:
			return create_boolean_(boolean_value(to_string() > operand->to_string()));
		default:
			break;
		}

		return basic::evaluate(info);
	}

	auto type = get_type();
	auto bully = (type->get_bully(operand_type.get()) == type.get()) ? type : operand_type;
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
		return create_boolean_(boolean_value(!is_nan() && !numeric_operand->is_nan() && compare_(bully, *numeric_operand) < 0));
	case lexer::operator_id::less_or_equal:
		return create_boolean_(boolean_value(!is_nan() && !numeric_operand->is_nan() && compare_(bully, *numeric_operand) <= 0));
	case lexer::operator_id::equality:
		return create_boolean_(boolean_value(is_nan() == numeric_operand->is_nan() && compare_(bully, *numeric_operand) == 0));
	case lexer::operator_id::inverse_equality:
		return create_boolean_(boolean_value(is_nan() != numeric_operand->is_nan() || compare_(bully, *numeric_operand) != 0));
	case lexer::operator_id::more_or_equal:
		return create_boolean_(boolean_value(!is_nan() && !numeric_operand->is_nan() && compare_(bully, *numeric_operand) >= 0));
	case lexer::operator_id::more:
		return create_boolean_(boolean_value(!is_nan() && !numeric_operand->is_nan() && compare_(bully, *numeric_operand) > 0));
	default:
		break;
	}

	return basic::evaluate(info);
}

cscript::object::generic *cscript::object::primitive::numeric::evaluate(const unary_info &info){
	if (is_uninitialized())
		return basic::evaluate(info);

	if (info.left){
		switch (info.id){
		case lexer::operator_id::plus:
			return plus_();
		case lexer::operator_id::minus:
			return minus_();
		case lexer::operator_id::bitwise_inverse:
			return bitwise_inverse_();
		case lexer::operator_id::decrement:
			common::env::runtime.operand.object = common::env::get_integer();
			return subtract_(nullptr, *common::env::runtime.operand.object->query<numeric>(), true);
		case lexer::operator_id::increment:
			common::env::runtime.operand.object = common::env::get_integer();
			return add_(nullptr, *common::env::runtime.operand.object->query<numeric>(), true);
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
			common::env::runtime.operand.object = common::env::get_integer();
			subtract_(nullptr, *common::env::runtime.operand.object->query<numeric>(), true);

			return old;
		}
		case lexer::operator_id::increment:
		{
			if (is_constant() || !is_lvalue())
				return common::env::error.set("Operator does not take specified operand");

			auto old = clone();
			common::env::runtime.operand.object = common::env::get_integer();
			add_(nullptr, *common::env::runtime.operand.object->query<numeric>(), true);

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
	return CSCRIPT_IS(get_memory().attributes, memory::virtual_address::attribute::is_nan);
}

cscript::object::primitive::boolean::value_type cscript::object::primitive::numeric::boolean_value(bool value){
	return value ? boolean::value_type::true_ : boolean::value_type::false_;
}

cscript::object::generic *cscript::object::primitive::numeric::post_assignment_(generic &operand){
	if (CSCRIPT_IS(operand.get_memory().attributes, memory::virtual_address::attribute::is_nan))
		CSCRIPT_SET(get_memory().attributes, memory::virtual_address::attribute::is_nan);
	else
		CSCRIPT_REMOVE(get_memory().attributes, memory::virtual_address::attribute::is_nan);

	return this;
}

cscript::object::generic::ptr_type cscript::object::primitive::numeric::clone_(const type::generic::ptr_type type, bool is_nan){
	auto value = std::make_shared<numeric>(type);
	if (is_nan)
		CSCRIPT_SET(value->get_memory().attributes, memory::virtual_address::attribute::is_nan);

	return value;
}

cscript::object::generic *cscript::object::primitive::numeric::create_boolean_(boolean::value_type value){
	if (common::env::error.has())
		return nullptr;

	return common::env::temp_storage.add(std::make_shared<boolean>(value));
}

cscript::object::generic *cscript::object::primitive::numeric::plus_(){
	if (is_uninitialized())
		return common::env::error.set("Uninitialized object in expression");

	auto op = get_op_();
	auto &memory_entry = get_memory();
	auto destination = common::env::temp_storage.add(clone_(memory_entry.info.type, is_nan()));

	CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
	if (!is_nan())
		op->plus(destination->get_memory().base, memory_entry.base);

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::minus_(){
	auto &memory_entry = get_memory();
	if (memory_entry.info.type->is_unsigned_integral())
		return common::env::error.set("Operator does not take specified operand");

	if (is_uninitialized())
		return common::env::error.set("Uninitialized object in expression");

	auto op = get_op_();
	auto destination = common::env::temp_storage.add(clone_(memory_entry.info.type, is_nan()));

	CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
	if (!is_nan())
		op->minus(destination->get_memory().base, memory_entry.base);

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::bitwise_inverse_(){
	auto &memory_entry = get_memory();
	if (!memory_entry.info.type->is_integral())
		return common::env::error.set("Operator does not take specified operand");

	if (is_uninitialized())
		return common::env::error.set("Uninitialized object in expression");

	auto op = get_op_();
	auto destination = common::env::temp_storage.add(clone_(memory_entry.info.type, is_nan()));

	CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
	if (!is_nan())
		op->bitwise_inverse(destination->get_memory().base, memory_entry.base);

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
			if (operand.compare(0) == 0)//Division by zero
				CSCRIPT_SET(destination->get_memory().attributes, memory::virtual_address::attribute::is_nan);
			else
				op->divide(destination->get_memory().base, left, right);
		}
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::modulus_(type::generic::ptr_type bully, numeric &operand, bool assign){
	if (!get_type()->is_integral() || !operand.get_type()->is_integral())
		return common::env::error.set("Operator does not take specified operand");

	object::generic *destination;
	memory::pool::base_type left, right;

	auto op = prepare_(bully, operand, assign, destination, left, right);
	if (destination != nullptr){
		CSCRIPT_REMOVE(destination->get_memory().attributes, memory::virtual_address::attribute::uninitialized);
		if (!is_nan() && !operand.is_nan()){
			if (operand.compare(0) == 0)//Division by zero
				CSCRIPT_SET(destination->get_memory().attributes, memory::virtual_address::attribute::is_nan);
			else
				op->modulus(destination->get_memory().base, left, right);
		}
	}

	return destination;
}

cscript::object::generic *cscript::object::primitive::numeric::left_shift_(type::generic::ptr_type bully, numeric &operand, bool assign){
	if (!get_type()->is_integral() || !operand.get_type()->is_integral())
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
	if (!get_type()->is_integral() || !operand.get_type()->is_integral())
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
	if (!get_type()->is_integral() || !operand.get_type()->is_integral())
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
	if (!get_type()->is_integral() || !operand.get_type()->is_integral())
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
	if (!get_type()->is_integral() || !operand.get_type()->is_integral())
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
	if (bully.get() == get_type().get()){
		left = get_memory().base;
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

		bully = get_type();
		destination = this;
	}
	else//Create temp
		destination = common::env::temp_storage.add(clone_(bully, is_nan() || operand.is_nan()));

	block_operator_type op;
	if (bully.get() == get_type().get()){
		left = get_memory().base;
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
	switch (get_memory().info.type->get_id()){
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
	if (is_uninitialized()){
		common::env::error.set("Uninitialized value in expression");
		return "";
	}

	if (is_nan())
		return "nan";

	switch (get_memory().info.type->get_id()){
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
		return common::env::real_to_string(get_value<float>()) + (echo ? "f" : "");
	case type::id::double_:
		return common::env::real_to_string(get_value<double>());
	case type::id::ldouble:
		return common::env::real_to_string(get_value<long double>()) + (echo ? "l" : "");
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

cscript::object::primitive::numeric_ref::numeric_ref(memory::virtual_address::value_type memory_value,
	const type::generic::ptr_type type, bool is_constant)
	: numeric(false), memory_(common::env::address_space.get_bound_entry(memory_value)){
	memory_value_ = memory_value;
	memory_.info.type = type;
	if (is_constant)
		CSCRIPT_SET(memory_.attributes, memory::virtual_address::attribute::constant);
}

cscript::object::primitive::numeric_ref::~numeric_ref(){}

cscript::memory::virtual_address::entry &cscript::object::primitive::numeric_ref::get_memory(){
	return memory_;
}

bool cscript::object::primitive::numeric_ref::is_lvalue(){
	return true;
}
