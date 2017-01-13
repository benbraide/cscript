#pragma once

#ifndef CSCRIPT_EXTERNAL_FUNCTION_DEFINITION_H
#define CSCRIPT_EXTERNAL_FUNCTION_DEFINITION_H

#include <functional>

#include "function_definition.h"

namespace cscript{
	namespace function{
		class external_definition : public definition{
		public:
			typedef std::function<object::generic *(storage::generic *)> handler_type;

			explicit external_definition(handler_type handler);

			virtual ~external_definition();

			virtual object::generic *call(storage::generic *storage);

		protected:
			handler_type handler_;
		};
	}
}

#endif /* !CSCRIPT_EXTERNAL_FUNCTION_DEFINITION_H */
