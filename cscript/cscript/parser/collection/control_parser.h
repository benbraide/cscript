#pragma once

#ifndef CSCRIPT_CONTROL_PARSER_H
#define CSCRIPT_CONTROL_PARSER_H

#include "../generic_parser.h"
#include "../parser_context.h"

#include "../../node/iteration_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class control : public generic{
			public:
				virtual node_type parse() override;

			protected:
				template <class env_type, class control_type>
				node_type parse_control_(){
					//<tag> (<predicate>)[{] <body> [}]
					auto index = env_type::parser_info.token->get_index();

					lexer::auto_skip enable_skip(env_type::source_info, &lexer::token_id_compare_collection::skip);
					env_type::source_info.source->ignore(env_type::source_info);

					auto predicate = parse_predicate_(index);
					if (env_type::error.has())
						return nullptr;

					auto body = parse_body_(index);
					if (env_type::error.has())
						return nullptr;

					auto else_ = parse_else_();
					if (env_type::error.has())
						return nullptr;

					return std::make_shared<control_type>(index, predicate, body, else_);
				}

				node_type parse_do_while_();

				node_type parse_for_();

				node_type parse_try_or_finally_(bool is_finally);

				node_type parse_catch_or_finally_();

				virtual node_type parse_else_();

				virtual node_type parse_predicate_(const lexer::token::index &index);

				virtual node_type parse_body_(const lexer::token::index &index, bool block = false);
			};
		}
	}
}

#endif /* !CSCRIPT_CONTROL_PARSER_H */
