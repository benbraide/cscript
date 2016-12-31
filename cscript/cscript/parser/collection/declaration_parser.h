#pragma once

#ifndef CSCRIPT_DECLARATION_PARSER_H
#define CSCRIPT_DECLARATION_PARSER_H

#include "../generic_parser.h"
#include "../parser_context.h"

#include "../../node/identifier_node.h"
#include "../../node/declaration_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class declaration : public generic{
			public:
				virtual node_type parse() override;

			protected:
				virtual node_type extend_(node_type value);
			};
		}
	}
}

#endif /* !CSCRIPT_DECLARATION_PARSER_H */
