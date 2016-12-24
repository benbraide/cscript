#include "env.h"

thread_local cscript::object::generic *cscript::common::env::object_operand;

thread_local cscript::node::generic *cscript::common::env::node_operand;

thread_local std::shared_ptr<cscript::lexer::source_info> cscript::common::env::source_info;

thread_local cscript::parser::parser_info cscript::common::env::parser_info{};

thread_local cscript::common::error cscript::common::env::error;

thread_local cscript::storage::temp cscript::common::env::temp_storage;

thread_local cscript::memory::static_block cscript::common::env::static_block1;

thread_local cscript::memory::static_block cscript::common::env::static_block2;

cscript::memory::virtual_address cscript::common::env::address_space;

cscript::memory::temp_virtual_address cscript::common::env::temp_address_space;

const cscript::type::generic::ptr_type cscript::common::env::any_type = std::make_shared<type::primitive>(type::id::any);

const cscript::type::generic::ptr_type cscript::common::env::auto_type = std::make_shared<type::primitive>(type::id::auto_);

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

cscript::object::generic *cscript::common::env::get_object_operand(){
	if (object_operand != nullptr || node_operand == nullptr)
		return object_operand;

	return (object_operand = temp_storage.add(node_operand->evaluate()));
}
