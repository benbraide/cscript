#pragma once

#ifndef CSCRIPT_GENERIC_OBJECT_H
#define CSCRIPT_GENERIC_OBJECT_H

#include "../memory/virtual_address.h"

namespace cscript{
	namespace object{
		class generic{
		public:
			virtual ~generic(){}

			virtual memory::virtual_address::entry &get_memory() = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_OBJECT_H */
