#pragma once

#ifndef CSCRIPT_BASIC_OBJECT_H
#define CSCRIPT_BASIC_OBJECT_H

#include "generic_object.h"

namespace cscript{
	namespace object{
		class basic : public generic{
		public:
			explicit basic(memory::virtual_address::entry &memory);

			virtual ~basic();

			virtual memory::virtual_address::entry &get_memory() override;

		protected:
			memory::virtual_address::entry &memory_;
		};
	}
}

#endif /* !CSCRIPT_BASIC_OBJECT_H */
