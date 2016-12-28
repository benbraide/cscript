#pragma once

#ifndef CSCRIPT_BINARY_OPERATOR_PARSER_H
#define CSCRIPT_BINARY_OPERATOR_PARSER_H

#include "../generic_parser.h"
#include "../parser_context.h"

#include "../../node/binary_operator_node.h"
#include "../../node/unary_operator_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class binary_operator : public generic{
			public:
				virtual node_type parse() override;

			protected:
				virtual int get_precedence_(lexer::operator_token &token);

				virtual int get_unary_precedence_(lexer::operator_token &token);
			};
		}
	}
}

#endif /* !CSCRIPT_BINARY_OPERATOR_PARSER_H */
