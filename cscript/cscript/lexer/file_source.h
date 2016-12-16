#pragma once

#ifndef CSCRIPT_FILE_SOURCE_H
#define CSCRIPT_FILE_SOURCE_H

#include <boost/iostreams/device/mapped_file.hpp>

#include "common_source.h"

namespace cscript{
	namespace lexer{
		namespace source{
			class file : public common<const char *>{
			public:
				explicit file(const std::string &path);

				virtual bool is_open() const;

			protected:
				boost::iostreams::mapped_file buffer_;
			};
		}
	}
}

#endif /* !CSCRIPT_FILE_SOURCE_H */
