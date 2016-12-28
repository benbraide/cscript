#pragma once

#ifndef CSCRIPT_GENERIC_CONTEXT_H
#define CSCRIPT_GENERIC_CONTEXT_H

#include "parser_id.h"

#include "../node/node_id.h"
#include "../lexer/token_id.h"

namespace cscript{
	namespace parser{
		class generic_context{
		public:
			virtual ~generic_context(){}

			virtual generic_context *get_parent() = 0;

			virtual bool accepts(node::id id) const = 0;

			virtual bool is_block() const = 0;

			template <typename object_type>
			object_type *query(){
				return dynamic_cast<object_type *>(this);
			}

			template <typename object_type>
			const object_type *query() const{
				return dynamic_cast<const object_type *>(this);
			}
		};
	}
}

#endif /* !CSCRIPT_GENERIC_CONTEXT_H */
