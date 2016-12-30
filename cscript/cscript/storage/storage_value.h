#pragma once

#ifndef CSCRIPT_STORAGE_VALUE_H
#define CSCRIPT_STORAGE_VALUE_H

#include "generic_storage_value.h"

#include "../type/generic_type.h"
#include "../object/generic_object.h"

namespace cscript{
	namespace storage{
		class value : public generic_value{
		public:
			struct info{
				generic *storage;
				type_ptr type;
				object_type *object;
			};

			value();

			explicit value(generic &storage);

			explicit value(type_ptr type);

			explicit value(object_type &object);

			virtual void set(generic &storage);

			virtual void set(type_ptr type);

			virtual void set(object_type &object);

			virtual generic *get_storage() override;

			virtual type_ptr get_type() override;

			virtual object_type *get_object() override;

			virtual bool is_storage() const override;

			virtual bool is_type() const override;

		protected:
			info info_{};
		};
	}
}

#endif /* !CSCRIPT_STORAGE_VALUE_H */
