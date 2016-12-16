#pragma once

#ifndef CSCRIPT_GENERIC_TOKEN_ID_COMPARE_H
#define CSCRIPT_GENERIC_TOKEN_ID_COMPARE_H

#include "token_id.h"

namespace cscript{
	namespace lexer{
		class generic_token_id_compare{
		public:
			virtual ~generic_token_id_compare(){}

			virtual bool is(token_id value) const = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_TOKEN_ID_COMPARE_H */
