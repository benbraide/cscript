#pragma once

#ifndef CSCRIPT_POINTER_OBJECT_H
#define CSCRIPT_POINTER_OBJECT_H

#include "basic_object.h"

namespace cscript{
	namespace object{
		class pointer : public basic{
		public:
			typedef memory::virtual_address::value_info value_type;

			explicit pointer(const value_type &value);

			pointer(memory::virtual_address &address_space, const value_type &value);

			virtual ~pointer();

			virtual generic *clone() override;

			virtual generic *evaluate(const binary_info &info) override;

			virtual generic *evaluate(const unary_info &info) override;

			virtual bool to_bool() override;
		};
	}
}

#endif /* !CSCRIPT_POINTER_OBJECT_H */
