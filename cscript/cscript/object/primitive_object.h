#pragma once

#ifndef CSCRIPT_PRIMITIVE_OBJECT_H
#define CSCRIPT_PRIMITIVE_OBJECT_H

#include "../common/env.h"
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
				explicit numeric(const type::generic::ptr_type type);

				template <typename value_type>
				numeric(const type::generic::ptr_type type, value_type value)
					: generic(common::env::address_space.add<value_type>()){
					memory_.type = type;
					memory::pool::write_unchecked(memory_.base, value);
				}

				virtual ~numeric();
			};
		}
	}
}

#endif /* !CSCRIPT_PRIMITIVE_OBJECT_H */
