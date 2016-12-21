#pragma once

#ifndef CSCRIPT_GENERIC_SCANNER_H
#define CSCRIPT_GENERIC_SCANNER_H

#include "generic_source.h"

namespace cscript{
	namespace lexer{
		class generic_scanner : public generic_source{
		public:
			virtual ~generic_scanner(){}

			virtual generic_scanner &pre_cache(source_info &info, int max_size = 0x03ff, int min_size = 0x00ff) = 0;

			virtual generic_scanner &save(token_type value) = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_SCANNER_H */
