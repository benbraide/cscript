#pragma once

#ifndef CSCRIPT_STORAGE_CONTAINER_H
#define CSCRIPT_STORAGE_CONTAINER_H

#include "../object/generic_object.h"

namespace cscript{
	namespace storage{
		class container{
		public:
			virtual ~container(){}

			virtual object::generic::ptr_type get_ptr(const object::generic *object) = 0;
		};
	}
}

#endif /* !CSCRIPT_STORAGE_CONTAINER_H */
