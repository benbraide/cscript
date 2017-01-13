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

			static object::generic *is_block_head(storage::generic *storage);

			static object::generic *get_block_size(storage::generic *storage);

			static object::generic *get_block_object(storage::generic *storage);

			static object::generic *get_block_type(storage::generic *storage);

			static object::generic *copy(storage::generic *storage);

			static object::generic *set(storage::generic *storage);

			static object::generic *move(storage::generic *storage);

			static object::generic *watch(storage::generic *storage);

			static object::generic *unwatch(storage::generic *storage);

			static object::generic *string_new(storage::generic *storage);

			static object::generic *string_resize(storage::generic *storage);

			static object::generic *string_insert(storage::generic *storage);

			static object::generic *string_erase(storage::generic *storage);

			static object::generic *string_clear(storage::generic *storage);

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
