#pragma once

#ifndef CSCRIPT_STRING_SOURCE_H
#define CSCRIPT_STRING_SOURCE_H

#include "common_source.h"

namespace cscript{
	namespace lexer{
		namespace source{
			class string : public common<std::string::const_iterator>{
			public:
				explicit string(const std::string &buffer);

				explicit string(std::string &&buffer);

				virtual void reset(const std::string &buffer);

			protected:
				std::string buffer_;
			};
		}
	}
}

#endif /* !CSCRIPT_STRING_SOURCE_H */
