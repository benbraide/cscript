#pragma once

#ifndef CSCRIPT_GENERIC_NODE_H
#define CSCRIPT_GENERIC_NODE_H

#include <string>

#include "node_id.h"
#include "../lexer/token.h"
#include "../object/generic_object.h"
#include "../storage/generic_storage.h"

namespace cscript{
	namespace node{
		class generic{
		public:
			typedef std::shared_ptr<generic> ptr_type;

			virtual ~generic(){}

			virtual ptr_type clone() = 0;

			virtual id get_id() const = 0;

			virtual const lexer::token::index &get_index() const = 0;

			virtual bool is(id id) const = 0;

			virtual std::string print() const = 0;

			virtual object::generic *evaluate() = 0;

			virtual std::string get_key() = 0;

			virtual storage::generic *get_storage() = 0;

			virtual type::generic::ptr_type get_type() = 0;

			template <typename object_type>
			object_type *query(){
				return dynamic_cast<object_type *>(this);
			}
		};
	}
}

#endif /* !CSCRIPT_GENERIC_NODE_H */
