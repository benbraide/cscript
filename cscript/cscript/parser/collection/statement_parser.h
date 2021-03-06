#pragma once

#ifndef CSCRIPT_STATEMENT_PARSER_H
#define CSCRIPT_STATEMENT_PARSER_H

#include "../generic_parser.h"
#include "../../node/statement_node.h"
#include "../../lexer/preprocessor_token.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class statement : public generic{
			public:
				virtual node_type parse() override;

			protected:
				virtual void ignore_inactive_block_();

				virtual node_type check_for_semicolon_(const lexer::token::index &index, node_type value, bool optional);
			};
		}
	}
}

#endif /* !CSCRIPT_STATEMENT_PARSER_H */
