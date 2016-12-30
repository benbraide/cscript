#pragma once

#ifndef CSCRIPT_GENERIC_STORAGE_VALUE_H
#define CSCRIPT_GENERIC_STORAGE_VALUE_H

#include <memory>

#include "generic_storage.h"

namespace cscript{
	namespace type{
		class generic;
	}

	namespace object{
		class generic;
	}

	namespace storage{
		class generic_value{
		public:
			typedef std::shared_ptr<type::generic> type_ptr;
			typedef object::generic object_type;

			virtual ~generic_value(){}

			virtual generic *get_storage() = 0;

			virtual type_ptr get_type() = 0;

			virtual object_type *get_object() = 0;

			virtual bool is_storage() const = 0;

			virtual bool is_type() const = 0;

			template <typename object_type>
			object_type *query(){
				return dynamic_cast<object_type *>(this);
			}
		};
	}
}

#endif /* !CSCRIPT_GENERIC_STORAGE_VALUE_H */
