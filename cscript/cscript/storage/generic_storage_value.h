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
			typedef object::generic object_type;
			typedef std::shared_ptr<object_type> object_ptr;
			typedef std::shared_ptr<type::generic> type_ptr;

			virtual ~generic_value(){}

			virtual void set(generic &storage) = 0;

			virtual void set(type_ptr type) = 0;

			virtual void set(object_ptr object) = 0;

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
