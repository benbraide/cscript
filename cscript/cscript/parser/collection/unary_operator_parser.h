#pragma once

#ifndef CSCRIPT_UNARY_OPERATOR_PARSER_H
#define CSCRIPT_UNARY_OPERATOR_PARSER_H

#include "../generic_parser.h"
#include "../parser_context.h"

#include "../../node/unary_operator_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class unary_operator : public generic{
			public:
				virtual node_type parse() override;

			protected:
				virtual int get_precedence_(lexer::operator_token &token);
			};
		}
	}
}

#endif /* !CSCRIPT_UNARY_OPERATOR_PARSER_H */
