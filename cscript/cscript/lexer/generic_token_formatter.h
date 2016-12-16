#pragma once

#ifndef CSCRIPT_GENERIC_TOKEN_FORMATTER_H
#define CSCRIPT_GENERIC_TOKEN_FORMATTER_H

#include <memory>

#include "rule.h"
#include "token.h"
#include "generic_token_id_compare.h"

namespace cscript{
	namespace lexer{
		class generic_source;
		struct source_info;
		
		class generic_token_formatter{
		public:
			typedef std::shared_ptr<token> token_type;

			struct match_info{
				token::index index;
				std::string value;
				int match_index;
				token::adjustment adjustment;
			};

			virtual ~generic_token_formatter(){}

			virtual token_type format(match_info &match, source_info &info) = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_TOKEN_FORMATTER_H */
