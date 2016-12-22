#pragma once

#ifndef CSCRIPT_GENERIC_PARSER_H
#define CSCRIPT_GENERIC_PARSER_H

#include <memory>

#include "../node/generic_node.h"
#include "../lexer/generic_scanner.h"

#include "generic_context.h"

namespace cscript{
	namespace parser{
		struct parser_info;

		class generic{
		public:
			typedef std::shared_ptr<node::generic> node_type;

			virtual ~generic(){}

			virtual node_type parse() = 0;
		};

		struct parser_info{
			lexer::generic_source::token_type token;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_PARSER_H */
