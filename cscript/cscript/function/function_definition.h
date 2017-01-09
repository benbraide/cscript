#pragma once

#ifndef CSCRIPT_FUNCTION_DEFINITION_H
#define CSCRIPT_FUNCTION_DEFINITION_H

#include "generic_function.h"
#include "../node/generic_node.h"

namespace cscript{
	namespace function{
		class definition{
		public:
			typedef node::generic::ptr_type node_type;

			explicit definition(node_type parameters, node_type value);

			virtual ~definition();

			virtual object::generic *call(storage::generic *storage);

			virtual node_type get_value() const;

			virtual node_type get_parameters() const;

		protected:
			node_type parameters_;
			node_type value_;
		};
	}
}

#endif /* !CSCRIPT_FUNCTION_DEFINITION_H */
