#pragma once

#ifndef CSCRIPT_KEYWORD_PARSER_H
#define CSCRIPT_KEYWORD_PARSER_H

#include "../generic_parser.h"
#include "../parser_context.h"

#include "../../node/type_cast_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class keyword : public generic{
			public:
				virtual node_type parse() override;

			protected:
				virtual node_type parse_type_cast_();

				virtual node_type parse_placeholder_();

				virtual node_type parse_operator_();

				virtual node_type parse_array_type_();

				virtual node_type parse_pointer_type_();

				virtual node_type parse_function_type_();

				virtual node_type parse_type_(const lexer::token::index &index);

				virtual node_type parse_value_(const lexer::token::index &index);
			};
		}
	}
}

#endif /* !CSCRIPT_KEYWORD_PARSER_H */
