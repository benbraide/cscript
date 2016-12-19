#pragma once

#ifndef CSCRIPT_TOKEN_FORMATTER_H
#define CSCRIPT_TOKEN_FORMATTER_H

#include "generic_token_formatter.h"
#include "generic_source.h"
#include "token_id_compare.h"
#include "operator_symbol.h"
#include "preprocessor_token.h"

namespace cscript{
	namespace lexer{
		namespace formatter{
			class linkable_token_formatter : public generic_token_formatter{
			public:
				explicit linkable_token_formatter(const generic_token_formatter *next);

			protected:
				virtual creator_type call_next_(match_info &match, source_info &info) const;

				const generic_token_formatter *next_;
			};

			class forward_slash : public linkable_token_formatter{
			public:
				explicit forward_slash(const generic_token_formatter *next = nullptr);

				virtual creator_type format(match_info &match, source_info &info) const override;

			protected:
				creator_type format_(match_info &match, source_info &info) const;
			};

			class string : public linkable_token_formatter{
			public:
				explicit string(const generic_token_formatter *next = nullptr);

				virtual creator_type format(match_info &match, source_info &info) const override;

			protected:
				creator_type format_(match_info &match, source_info &info, bool escaped) const;
			};

			class type : public linkable_token_formatter{
			public:
				typedef std::string::size_type size_type;

				explicit type(const generic_token_formatter *next = nullptr);

				virtual creator_type format(match_info &match, source_info &info) const override;

			protected:
				creator_type format_unsigned_(match_info &match, source_info &info) const;

				creator_type format_unsigned_long_(match_info &match, source_info &info) const;

				creator_type format_long_(match_info &match, source_info &info) const;

				token_type next_(match_info &match, source_info &info, int &count, size_type &blanks) const;
			};

			class operator_symbol : public linkable_token_formatter{
			public:
				explicit operator_symbol(const generic_token_formatter *next = nullptr);

				virtual creator_type format(match_info &match, source_info &info) const override;

				static const lexer::operator_symbol symbols;
			};

			class preprocessor : public linkable_token_formatter{
			public:
				explicit preprocessor(const generic_token_formatter *next = nullptr);

				virtual creator_type format(match_info &match, source_info &info) const override;

			protected:
				creator_type format_include_(match_info &match, source_info &info) const;

				creator_type format_define_(match_info &match, source_info &info) const;

				creator_type format_if_defined_(match_info &match, source_info &info) const;

				creator_type format_if_not_defined_(match_info &match, source_info &info) const;

				creator_type format_else_(match_info &match, source_info &info) const;

				creator_type format_end_if_(match_info &match, source_info &info) const;

				creator_type format_pragma_(match_info &match, source_info &info) const;
			};

			class collection{
			public:
				static const forward_slash					forward_slash;
				static const string							string;
				static const type							type;
				static const operator_symbol				operator_symbol;
				static const preprocessor					preprocessor;
			};

			class linked_collection{
			public:
				static const operator_symbol				operator_symbol;
				static const forward_slash					forward_slash;
				static const string							string;
				static const type							type;
				static const preprocessor					preprocessor;
				static const generic_token_formatter&		last;
			};
		}
	}
}

#endif /* !CSCRIPT_TOKEN_FORMATTER_H */
