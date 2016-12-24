#pragma once

#ifndef CSCRIPT_GENERIC_NODE_H
#define CSCRIPT_GENERIC_NODE_H

#include <string>

#include "node_id.h"
#include "../lexer/token.h"
#include "../object/generic_object.h"

namespace cscript{
	namespace node{
		class generic{
		public:
			virtual ~generic(){}

			virtual const generic *get_parent() const = 0;

			virtual generic *get_parent() = 0;

			virtual id get_id() const = 0;

			virtual const lexer::token::index &get_index() const = 0;

			virtual bool is(id id) const = 0;

			virtual std::string print() const = 0;

			virtual int get_print_tab() const = 0;

			virtual object::generic::ptr_type evaluate() = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_NODE_H */
