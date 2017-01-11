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

				typedef std::function<bool(node_type &)> callback_type;

				struct options{
					generic &parser;
					halt_info terminator;
					halt_info delimiter;
					bool no_trailing_delimiter;
					callback_type before_callback;
					callback_type after_callback;
					node::id alternate_delimiter;
				};

				static node_type parse(const options &options);

				static node_type parse_parameters(const halt_info &terminator = halt_info{ lexer::token_id::close_par });

				static node_type parse_list(const halt_info &terminator, const halt_info &delimiter, bool no_trailing_delimiter);

				static node_type parse_list(const halt_info &terminator, bool no_trailing_delimiter);

				static node_type parse_list(const halt_info &terminator);

				static node_type parse_block(const halt_info &terminator, callback_type before_callback, callback_type after_callback);

				static node_type parse_block(const halt_info &terminator = halt_info{ lexer::token_id::close_cur });

				static node_type parse_single(const halt_info &terminator);

				static node_type parse_type();

				static node_type parse_expression();

				static node_type parse_name();

				static int get_match_type(const halt_info &terminator, const halt_info &delimiter);
			};
		}
	}
}

#endif /* !CSCRIPT_COLLECTION_PARSER_H */
