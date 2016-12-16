#pragma once

#ifndef CSCRIPT_TOKEN_FORMATTER_H
#define CSCRIPT_TOKEN_FORMATTER_H

#include "generic_token_formatter.h"
#include "generic_source.h"

namespace cscript{
	namespace lexer{
		namespace formatter{
			class forward_slash : public generic_token_formatter{
			public:
				virtual token_type format(match_info &match, source_info &info) override;

			protected:
				token_type format_(match_info &match, source_info &info);
			};

			class string : public forward_slash{
			public:
				virtual token_type format(match_info &match, source_info &info) override;

			protected:
				token_type format_(match_info &match, source_info &info);
			};
		}
	}
}

#endif /* !CSCRIPT_TOKEN_FORMATTER_H */
