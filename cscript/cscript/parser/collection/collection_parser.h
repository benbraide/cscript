#pragma once

#ifndef CSCRIPT_COLLECTION_PARSER_H
#define CSCRIPT_COLLECTION_PARSER_H

#include <functional>

#include "../generic_parser.h"
#include "../parser_context.h"
#include "../../node/collection_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class builder{
			public:
				typedef generic::node_type node_type;
				typedef lexer::token_id token_id_type;
				typedef lexer::generic_source::token_type token_type;
				typedef lexer::source_info::halt_info halt_info;

				typedef std::function<bool()> before_callback_type;
				typedef std::function<bool(node_type &)> after_callback_type;

				struct options{
					generic &parser;
					halt_info terminator;
					halt_info delimiter;
					bool no_trailing_delimiter;
					before_callback_type before_callback;
					after_callback_type after_callback;
				};

				static node_type parse(const options &options);

				static node_type parse_list(const halt_info &terminator, const halt_info &delimiter, bool no_trailing_delimiter);

				static node_type parse_list(const halt_info &terminator, bool no_trailing_delimiter);

				static node_type parse_list(const halt_info &terminator);

				static node_type parse_block(const halt_info &terminator, before_callback_type before_callback, after_callback_type after_callback);

				static node_type parse_block(const halt_info &terminator);

				static node_type parse_type();
			};
		}
	}
}

#endif /* !CSCRIPT_COLLECTION_PARSER_H */
