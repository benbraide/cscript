#pragma once

#ifndef CSCRIPT_ENV_H
#define CSCRIPT_ENV_H

#include <memory>

#include "error.h"

#include "../lexer/generic_source.h"
#include "../parser/generic_parser.h"
#include "../type/primitive_type.h"
#include "../storage/temp_storage.h"

namespace cscript{
	namespace common{
		class env{
		public:
			static thread_local object::generic *object_operand;
			static thread_local node::generic *node_operand;

			static thread_local std::shared_ptr<lexer::source_info> source_info;
			static thread_local parser::parser_info parser_info;
			static thread_local error error;

			static thread_local storage::temp temp_storage;

			static thread_local memory::static_block static_block1;
			static thread_local memory::static_block static_block2;

			static memory::virtual_address address_space;
			static memory::temp_virtual_address temp_address_space;

			static const type::generic::ptr_type any_type;
			static const type::generic::ptr_type auto_type;

			static const type::generic::ptr_type bool_type;
			static const type::generic::ptr_type bit_type;

			static const type::generic::ptr_type byte_type;
			static const type::generic::ptr_type wchar_type;

			static const type::generic::ptr_type char_type;
			static const type::generic::ptr_type uchar_type;

			static const type::generic::ptr_type short_type;
			static const type::generic::ptr_type ushort_type;

			static const type::generic::ptr_type int_type;
			static const type::generic::ptr_type uint_type;

			static const type::generic::ptr_type long_type;
			static const type::generic::ptr_type ulong_type;

			static const type::generic::ptr_type llong_type;
			static const type::generic::ptr_type ullong_type;

			static const type::generic::ptr_type float_type;
			static const type::generic::ptr_type double_type;
			static const type::generic::ptr_type ldouble_type;

			static const object::generic::ptr_type zero;
			static const object::generic::ptr_type one;

			static object::generic *get_object_operand();
		};
	}
}

#endif /* !CSCRIPT_ENV_H */
