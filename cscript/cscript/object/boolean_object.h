#pragma once

#ifndef CSCRIPT_BOOLEAN_OBJECT_H
#define CSCRIPT_BOOLEAN_OBJECT_H

#include "basic_object.h"

namespace cscript{
	namespace object{
		namespace primitive{
			class boolean : public basic{
			public:
				typedef type::boolean_value_type value_type;

				explicit boolean(const type::generic::ptr_type type);

				boolean(const type::generic::ptr_type type, value_type value);

				boolean(memory::virtual_address &address_space, const type::generic::ptr_type type);

				boolean(memory::virtual_address &address_space, const type::generic::ptr_type type, value_type value);

				virtual ~boolean();
			};
		}
	}
}

#endif /* !CSCRIPT_BOOLEAN_OBJECT_H */
