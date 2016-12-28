#pragma once

#ifndef CSCRIPT_GENERIC_TOKEN_FORMATTER_H
#define CSCRIPT_GENERIC_TOKEN_FORMATTER_H

#include <memory>
#include <functional>

#include "rule.h"
#include "token.h"
#include "generic_token_id_compare.h"

namespace cscript{
	namespace lexer{
		class generic_source;
		class source_info;
		
		class generic_token_formatter{
		public:
			struct match_info{
				token::index index;
				const char *start;
				std::string::size_type size;
				int match_index;
				token::adjustment adjustment;
			};

			typedef std::shared_ptr<token> token_type;
			typedef std::function<token_type(match_info &)> creator_type;

			virtual ~generic_token_formatter(){}

			virtual creator_type format(match_info &match, source_info &info) const = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_TOKEN_FORMATTER_H */
