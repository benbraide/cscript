#pragma once

#ifndef CSCRIPT_PRIMITIVE_OBJECT_H
#define CSCRIPT_PRIMITIVE_OBJECT_H

#include "basic_object.h"

namespace cscript{
	namespace object{
		namespace primitive{
			class generic : public basic{
			public:
				explicit generic(memory::virtual_address::entry &memory);

				virtual ~generic();
			};

			class numeric : public generic{
			public:
				numeric(memory::virtual_address &target_memory, memory::virtual_address::size_type size);

				template <typename value_type>
				explicit numeric(memory::virtual_address &target_memory, value_type value)
					: generic(target_memory.add<value_type>()){
					memory::pool::write_unchecked(memory_.base, value);
				}

				virtual ~numeric();
			};
		}
	}
}

#endif /* !CSCRIPT_PRIMITIVE_OBJECT_H */
