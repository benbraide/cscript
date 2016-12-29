#pragma once

#ifndef CSCRIPT_TERM_PARSER_H
#define CSCRIPT_TERM_PARSER_H

#include "../generic_parser.h"
#include "../../node/unary_operator_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class term : public generic{
			public:
				virtual node_type parse() override;
			};
		}
	}
}

#endif /* !CSCRIPT_TERM_PARSER_H */
