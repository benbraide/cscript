#pragma once

#ifndef CSCRIPT_GENERIC_SCANNER_H
#define CSCRIPT_GENERIC_SCANNER_H

#include "generic_source.h"

namespace cscript{
	namespace lexer{
		class generic_scanner : public generic_source{
		public:
			virtual ~generic_scanner(){}

			virtual generic_scanner &save(token_type value) = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_SCANNER_H */
