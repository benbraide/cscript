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

				boolean();

				explicit boolean(value_type value);

				explicit boolean(memory::virtual_address &address_space);

				boolean(memory::virtual_address &address_space, value_type value);

				explicit boolean(memory::virtual_address::entry &parent);

				virtual ~boolean();

				virtual generic *clone() override;

				virtual generic *evaluate(const binary_info &info) override;

				virtual bool to_bool() override;

			protected:
				virtual value_type get_value_();
			};
		}
	}
}

#endif /* !CSCRIPT_BOOLEAN_OBJECT_H */
