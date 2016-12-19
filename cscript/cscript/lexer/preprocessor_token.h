#pragma once

#ifndef CSCRIPT_PREPROCESSOR_TOKEN_H
#define CSCRIPT_PREPROCESSOR_TOKEN_H

#include <list>
#include <memory>

#include "token.h"

namespace cscript{
	namespace lexer{
		namespace preprocessor_token{
			class include : public token{
			public:
				include(const index &index, const std::string &value, int match_index, bool is_absolute);

				include(const index &index, std::string &&value, int match_index, bool is_absolute);

				bool is_absolute() const;

			private:
				bool is_absolute_;
			};

			class define : public token{
			public:
				typedef std::shared_ptr<token> token_type;
				typedef std::list<token_type> list_type;

				define(const index &index, const std::string &value, int match_index, const list_type &list);

				define(const index &index, const std::string &value, int match_index, list_type &&list);

				const list_type &get_list() const;

				list_type &get_list();

			private:
				list_type list_;
			};

			class undefine : public token{
			public:
				undefine(const index &index, const std::string &value, int match_index);
			};
		}
	}
}

#endif /* !CSCRIPT_PREPROCESSOR_TOKEN_H */
