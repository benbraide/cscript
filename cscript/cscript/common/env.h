#pragma once

#ifndef CSCRIPT_ENV_H
#define CSCRIPT_ENV_H

#include <memory>
#include <iostream>

#include "error.h"
#include "runtime.h"

#include "../lexer/generic_source.h"
#include "../type/primitive_type.h"

#include "../storage/temp_storage.h"
#include "../storage/named_storage.h"

#include "../parser/collection/literal_parser.h"
#include "../parser/collection/term_parser.h"
#include "../parser/collection/unary_operator_parser.h"
#include "../parser/collection/binary_operator_parser.h"
#include "../parser/collection/expression_parser.h"
#include "../parser/collection/statement_parser.h"
#include "../parser/collection/declaration_parser.h"
#include "../parser/collection/function_parser.h"
#include "../parser/collection/keyword_parser.h"
#include "../parser/collection/collection_parser.h"

namespace cscript{
	namespace common{
		class env{
		public:
			static thread_local lexer::source_guard source_guard;
			static thread_local lexer::source_info source_info;
			static thread_local parser::parser_info parser_info;

			static thread_local storage::temp temp_storage;
			static storage::named global_storage;

			static thread_local error error;
			static thread_local runtime runtime;

			static thread_local memory::static_block static_block1;
			static thread_local memory::static_block static_block2;

			static memory::virtual_address address_space;

			static const type::generic::ptr_type any_type;
			static const type::generic::ptr_type auto_type;

			static const type::generic::ptr_type nullptr_type;
			static const type::generic::ptr_type void_type;

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

			static const type::generic::ptr_type string_type;

			static const type::generic::ptr_type type_object_type;
			static const type::generic::ptr_type node_object_type;
			static const type::generic::ptr_type storage_object_type;

			static const object::generic::ptr_type zero;
			static const object::generic::ptr_type one;

			static parser::collection::literal literal_parser;

			static parser::collection::unary_operator unary_operator_parser;
			static parser::collection::binary_operator binary_operator_parser;

			static parser::collection::term term_parser;
			static parser::collection::expression expression_parser;
			static parser::collection::statement statement_parser;

			static parser::collection::declaration declaration_parser;
			static parser::collection::function function_parser;

			static parser::collection::keyword keyword_parser;
			static parser::collection::builder builder;

			static object::generic *get_object_operand();

			static object::generic::ptr_type create_pointer(memory::address_value_type value, bool is_constant);

			static object::generic::ptr_type create_string(const std::string &value);

			static void initialize();

			static void echo(const std::string &value);

			static bool is_constant(object::generic &object);

			template <typename value_type>
			static std::string to_hex(value_type value, std::size_t width = sizeof(value_type) << 1) {
				static const char *digits = "0123456789abcdef";
				std::string rc(width, '0');
				for (size_t i = 0, j = (width - 1) * 4; i < width; ++i, j -= 4)
					rc[i] = digits[(value >> j) & 0x0f];

				return ("0x" + rc);
			}

			template <typename value_type>
			static std::string real_to_string(value_type value){
				auto string_value = std::to_string(value);
				auto index = string_value.size();
				for (; index > 1u; --index){
					if (string_value[index - 2] == '.' || string_value[index - 1] != '0')
						break;
				}

				if (index < string_value.size())
					string_value.erase(index);

				return string_value;
			}
		};
	}
}

#endif /* !CSCRIPT_ENV_H */
