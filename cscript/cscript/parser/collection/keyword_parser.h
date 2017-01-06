#pragma once

#ifndef CSCRIPT_KEYWORD_PARSER_H
#define CSCRIPT_KEYWORD_PARSER_H

#include "../generic_parser.h"
#include "../parser_context.h"

#include "../../node/identifier_node.h"
#include "../../node/decl_type_node.h"
#include "../../node/placeholder_node.h"
#include "../../node/operator_value_node.h"
#include "../../node/type_node.h"
#include "../../node/type_cast_node.h"
#include "../../node/typedef_node.h"
#include "../../node/echo_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class keyword : public generic{
			public:
				virtual node_type parse() override;

			protected:
				virtual node_type parse_typedef_();

				virtual node_type parse_type_cast_();

				virtual node_type parse_decl_type_();

				virtual node_type parse_placeholder_();

				virtual node_type parse_operator_();

				virtual node_type parse_symbol_operator_(const lexer::token::index &index,
					storage::operator_key &value, const std::string &operator_value, std::string &string_value);

				virtual node_type parse_symbol_operator_(const lexer::token::index &index, storage::operator_key &value,
					lexer::operator_id id, const std::string &operator_value, std::string &string_value);

				virtual node_type parse_type_operator_(const lexer::token::index &index,
					storage::operator_key &value);

				virtual node_type parse_array_type_();

				virtual node_type parse_pointer_type_();

				virtual node_type parse_function_type_();

				virtual node_type parse_echo_();

				virtual node_type parse_type_(const lexer::token::index &index, bool allow_storage_class);

				virtual node_type parse_value_(const lexer::token::index &index);
			};
		}
	}
}

#endif /* !CSCRIPT_KEYWORD_PARSER_H */
