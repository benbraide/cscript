#include "env.h"
#include "../object/pointer_object.h"

thread_local cscript::lexer::source_guard cscript::common::env::source_guard;

thread_local cscript::lexer::source_info cscript::common::env::source_info{};

thread_local cscript::parser::parser_info cscript::common::env::parser_info{};

thread_local cscript::storage::temp cscript::common::env::temp_storage;

cscript::storage::named cscript::common::env::global_storage("");

thread_local cscript::common::error cscript::common::env::error;

thread_local cscript::common::runtime cscript::common::env::runtime{ &global_storage };

thread_local cscript::memory::static_block cscript::common::env::static_block1;

thread_local cscript::memory::static_block cscript::common::env::static_block2;

cscript::memory::virtual_address cscript::common::env::address_space;

const cscript::type::generic::ptr_type cscript::common::env::any_type = std::make_shared<type::primitive>(type::id::any);

const cscript::type::generic::ptr_type cscript::common::env::auto_type = std::make_shared<type::primitive>(type::id::auto_);

const cscript::type::generic::ptr_type cscript::common::env::nullptr_type = std::make_shared<type::primitive>(type::id::nullptr_);

const cscript::type::generic::ptr_type cscript::common::env::void_type = std::make_shared<type::primitive>(type::id::void_);

const cscript::type::generic::ptr_type cscript::common::env::bool_type = std::make_shared<type::primitive>(type::id::bool_);

const cscript::type::generic::ptr_type cscript::common::env::bit_type = std::make_shared<type::primitive>(type::id::bit);

const cscript::type::generic::ptr_type cscript::common::env::byte_type = std::make_shared<type::primitive>(type::id::byte);

const cscript::type::generic::ptr_type cscript::common::env::wchar_type = std::make_shared<type::primitive>(type::id::wchar);

const cscript::type::generic::ptr_type cscript::common::env::char_type = std::make_shared<type::primitive>(type::id::char_);

const cscript::type::generic::ptr_type cscript::common::env::uchar_type = std::make_shared<type::primitive>(type::id::uchar);

const cscript::type::generic::ptr_type cscript::common::env::short_type = std::make_shared<type::primitive>(type::id::short_);

const cscript::type::generic::ptr_type cscript::common::env::ushort_type = std::make_shared<type::primitive>(type::id::ushort);

const cscript::type::generic::ptr_type cscript::common::env::int_type = std::make_shared<type::primitive>(type::id::int_);

const cscript::type::generic::ptr_type cscript::common::env::uint_type = std::make_shared<type::primitive>(type::id::uint);

const cscript::type::generic::ptr_type cscript::common::env::long_type = std::make_shared<type::primitive>(type::id::long_);

const cscript::type::generic::ptr_type cscript::common::env::ulong_type = std::make_shared<type::primitive>(type::id::ulong);

const cscript::type::generic::ptr_type cscript::common::env::llong_type = std::make_shared<type::primitive>(type::id::llong);

const cscript::type::generic::ptr_type cscript::common::env::ullong_type = std::make_shared<type::primitive>(type::id::ullong);

const cscript::type::generic::ptr_type cscript::common::env::float_type = std::make_shared<type::primitive>(type::id::float_);

const cscript::type::generic::ptr_type cscript::common::env::double_type = std::make_shared<type::primitive>(type::id::double_);

const cscript::type::generic::ptr_type cscript::common::env::ldouble_type = std::make_shared<type::primitive>(type::id::ldouble);

const cscript::type::generic::ptr_type cscript::common::env::string_type = std::make_shared<type::pointer>(char_type);

const cscript::type::generic::ptr_type cscript::common::env::type_object_type = std::make_shared<type::primitive>(type::id::type_);

const cscript::type::generic::ptr_type cscript::common::env::node_object_type = std::make_shared<type::primitive>(type::id::node_);

const cscript::type::generic::ptr_type cscript::common::env::storage_object_type = std::make_shared<type::primitive>(type::id::storage_);

const cscript::object::generic::ptr_type cscript::common::env::zero = std::make_shared<object::primitive::numeric>(int_type, 0);

const cscript::object::generic::ptr_type cscript::common::env::one = std::make_shared<object::primitive::numeric>(int_type, 1);

cscript::parser::collection::literal cscript::common::env::literal_parser;

cscript::parser::collection::unary_operator cscript::common::env::unary_operator_parser;

cscript::parser::collection::binary_operator cscript::common::env::binary_operator_parser;

cscript::parser::collection::term cscript::common::env::term_parser;

cscript::parser::collection::expression cscript::common::env::expression_parser;

cscript::parser::collection::statement cscript::common::env::statement_parser;

cscript::parser::collection::declaration cscript::common::env::declaration_parser;

cscript::parser::collection::function cscript::common::env::function_parser;

cscript::parser::collection::keyword cscript::common::env::keyword_parser;

cscript::parser::collection::builder cscript::common::env::builder;

cscript::object::generic *cscript::common::env::get_object_operand(){
	if (runtime.operand.object != nullptr || runtime.operand.node == nullptr)
		return runtime.operand.object;

	return (runtime.operand.object = runtime.operand.node->evaluate());
}

cscript::object::generic::ptr_type cscript::common::env::create_pointer(memory::address_value_type value, bool is_constant){
	auto pointer_object = std::make_shared<object::pointer>(value);
	if (is_constant)
		CSCRIPT_SET(pointer_object->get_memory().attributes, memory::address_attribute::constant);

	return pointer_object;
}

cscript::object::generic::ptr_type cscript::common::env::create_string(const std::string &value){
	return std::make_shared<object::pointer>(address_space.add(value));
}

void cscript::common::env::initialize(){
	static auto indeterminate = std::make_shared<object::primitive::boolean>(type::boolean_value_type::nil);
}

void cscript::common::env::echo(const std::string &value){
	std::cout << value << "\n";
}

bool cscript::common::env::is_constant(object::generic &object){
	auto storage = object.get_memory().info.storage;
	auto parent = (storage == nullptr) ? nullptr : storage->query<object::generic>();
	if (parent != nullptr && parent->is_constant())
		return true;

	return (std::find(runtime.operand.constant_objects.begin(), runtime.operand.constant_objects.end(), &object) !=
		runtime.operand.constant_objects.end());
}
