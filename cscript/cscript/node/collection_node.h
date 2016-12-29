#pragma once

#ifndef CSCRIPT_COLLECTION_NODE_H
#define CSCRIPT_COLLECTION_NODE_H

#include <list>

#include "basic_node.h"

namespace cscript{
	namespace node{
		class collection : public basic{
		public:
			typedef std::list<ptr_type> list_type;

			explicit collection(const lexer::token::index &index);

			virtual ~collection();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual collection &append(ptr_type value);

			virtual list_type &get_list();

		protected:
			list_type list_;
		};
	}
}

#endif /* !CSCRIPT_COLLECTION_NODE_H */
