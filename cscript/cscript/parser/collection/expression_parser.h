#pragma once

#ifndef CSCRIPT_EXPRESSION_PARSER_H
#define CSCRIPT_EXPRESSION_PARSER_H

#include "../generic_parser.h"
#include "../parser_context.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class expression : public generic{
			public:
				virtual node_type parse() override;
			};
		}
	}
}

#endif /* !CSCRIPT_EXPRESSION_PARSER_H */
