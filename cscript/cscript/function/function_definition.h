#pragma once

#ifndef CSCRIPT_FUNCTION_DEFINITION_H
#define CSCRIPT_FUNCTION_DEFINITION_H

#include "generic_function.h"
#include "../node/generic_node.h"

namespace cscript{
	namespace function{
		class definition{
		public:
			virtual ~definition();

			virtual object::generic *call();
		};
	}
}

#endif /* !CSCRIPT_FUNCTION_DEFINITION_H */
