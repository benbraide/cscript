#pragma once

#ifndef CSCRIPT_TEMP_STORAGE_H
#define CSCRIPT_TEMP_STORAGE_H

#include <list>

#include "../object/generic_object.h"

namespace cscript{
	namespace storage{
		class temp{
		public:
			typedef std::list<object::generic::ptr_type> list_type;

			virtual ~temp();

			virtual void clear();

			virtual object::generic *add(object::generic::ptr_type object);

		protected:
			list_type list_;
		};
	}
}

#endif /* !CSCRIPT_TEMP_STORAGE_H */
