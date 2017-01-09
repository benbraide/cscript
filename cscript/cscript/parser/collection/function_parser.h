#pragma once

#ifndef CSCRIPT_FUNCTION_PARSER_H
#define CSCRIPT_FUNCTION_PARSER_H

#include "../generic_parser.h"
#include "../parser_context.h"

#include "../../node/function_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class function : public generic{
			public:
				virtual node_type parse() override;

				virtual node_type parse_constructor();

				virtual node_type parse_destructor();
			};
		}
	}
}

#endif /* !CSCRIPT_FUNCTION_PARSER_H */
