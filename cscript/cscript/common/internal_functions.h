#pragma once

#ifndef CSCRIPT_INTERNAL_FUNCTIONS_H
#define CSCRIPT_INTERNAL_FUNCTIONS_H

#include "../function/external_function_definition.h"

namespace cscript{
	namespace common{
		class internal_functions{
		public:
			static object::generic *alloc(storage::generic *storage);

			static object::generic *dealloc(storage::generic *storage);

			static object::generic *realloc(storage::generic *storage);

			static object::generic *is_alloc(storage::generic *storage);

			static object::generic *is_valid_address(storage::generic *storage);

			static object::generic *move(storage::generic *storage);

			static object::generic *count(storage::generic *storage);

			static object::generic *begin(storage::generic *storage);

			static object::generic *end(storage::generic *storage);

			static object::generic *distance(storage::generic *storage);

			static object::generic *advance(storage::generic *storage);

		private:
			static std::nullptr_t validate_arguments_(int count);
		};
	}
}

#endif /* !CSCRIPT_INTERNAL_FUNCTIONS_H */
