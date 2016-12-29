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
			};
		}
	}
}

#endif /* !CSCRIPT_STATEMENT_PARSER_H */
