#pragma once

#ifndef CSCRIPT_DEFINED_SYMBOLS_H
#define CSCRIPT_DEFINED_SYMBOLS_H

#include <map>
#include <list>
#include <memory>

#include "token.h"
#include "../common/table.h"

namespace cscript{
	namespace lexer{
		class defined_symbols : public common::table<std::list<std::shared_ptr<token> > >{
		public:
			void add(const std::string &name, const value_type &value);

			void add(const std::string &name, value_type &&value);

			void remove(const std::string &name);

			const value_type *get_expansion(const std::string &name) const;

			bool is_defined(const std::string &name) const;
		};
	}
}

#endif /* !CSCRIPT_DEFINED_SYMBOLS_H */
