#pragma once

#ifndef CSCRIPT_GENERIC_RULE_H
#define CSCRIPT_GENERIC_RULE_H

#include <boost/regex.hpp>

#include "token_id.h"

namespace cscript{
	namespace lexer{
		class generic_rule{
		public:
			typedef std::string::const_iterator iterator_type;
			typedef boost::regex regex_type;

			struct match_info{
				int index;
				std::string::size_type size;
			};

			virtual ~generic_rule(){}

			virtual bool match(const std::string &value, match_info &matched) const = 0;

			virtual bool match(iterator_type start, iterator_type end, match_info &matched) const = 0;

			virtual bool match(const char *start, const char *end, match_info &matched) const = 0;

			virtual int get_error_index() const = 0;

			virtual token_id map_index(int index) const = 0;

			virtual const regex_type &get_compiled() const = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_RULE_H */
