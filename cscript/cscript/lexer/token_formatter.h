#pragma once

#ifndef CSCRIPT_TOKEN_FORMATTER_H
#define CSCRIPT_TOKEN_FORMATTER_H

#include "generic_token_formatter.h"
#include "generic_source.h"
#include "token_id_compare.h"

namespace cscript{
	namespace lexer{
		namespace formatter{
			class linkable_token_formatter : public generic_token_formatter{
			public:
				explicit linkable_token_formatter(const generic_token_formatter *next);

			protected:
				virtual token_type call_next_(match_info &match, source_info &info) const;

				const generic_token_formatter *next_;
			};

			class forward_slash : public linkable_token_formatter{
			public:
				explicit forward_slash(const generic_token_formatter *next = nullptr);

				virtual token_type format(match_info &match, source_info &info) const override;

			protected:
				token_type format_(match_info &match, source_info &info) const;
			};

			class string : public linkable_token_formatter{
			public:
				explicit string(const generic_token_formatter *next = nullptr);

				virtual token_type format(match_info &match, source_info &info) const override;

			protected:
				token_type format_(match_info &match, source_info &info, bool escaped) const;
			};

			class type : public linkable_token_formatter{
			public:
				explicit type(const generic_token_formatter *next = nullptr);

				virtual token_type format(match_info &match, source_info &info) const override;

			protected:
				token_type format_unsigned_(match_info &match, source_info &info) const;

				token_type format_unsigned_long_(match_info &match, source_info &info) const;

				token_type format_long_(match_info &match, source_info &info) const;

				token_type next_(match_info &match, source_info &info, int &count, std::string &blanks) const;
			};

			class keyword : public linkable_token_formatter{
			public:
				explicit keyword(const generic_token_formatter *next = nullptr);

				virtual token_type format(match_info &match, source_info &info) const override;
			};

			class collection{
			public:
				static const forward_slash	forward_slash;
				static const string			string;
				static const type			type;
			};

			class linked_collection{
			public:
				static const forward_slash					forward_slash;
				static const string							string;
				static const type							type;
				static const generic_token_formatter&		last;
			};
		}
	}
}

#endif /* !CSCRIPT_TOKEN_FORMATTER_H */
