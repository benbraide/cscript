#pragma once

#ifndef CSCRIPT_PREPROCESSOR_TOKEN_H
#define CSCRIPT_PREPROCESSOR_TOKEN_H

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
		}
	}
}

#endif /* !CSCRIPT_PREPROCESSOR_TOKEN_H */
