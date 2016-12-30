#pragma once

#ifndef CSCRIPT_POINTER_OBJECT_H
#define CSCRIPT_POINTER_OBJECT_H

#include "numeric_object.h"

namespace cscript{
	namespace object{
		class pointer : public basic{
		public:
			typedef memory::virtual_address::value_info value_type;

			explicit pointer(const type::generic::ptr_type type);

			explicit pointer(const value_type &value);

			pointer(memory::virtual_address &address_space, const value_type &value);

			pointer(memory::virtual_address &address_space, const value_type &value, type::generic::ptr_type type);

			pointer(memory::virtual_address::entry &parent, const type::generic::ptr_type type);

			virtual ~pointer();

			virtual generic *clone() override;

			virtual generic *cast(const type::generic *type) override;

			virtual generic *evaluate(const binary_info &info) override;

			virtual generic *evaluate(const unary_info &info) override;

			virtual bool to_bool() override;

			template <typename value_type>
			value_type get_value(){
				auto &value = get_value_();
				return static_cast<value_type>(value.address->convert_info(value));
			}

		protected:
			virtual generic *offset_(bool increment);

			virtual value_type &get_value_();
		};
	}
}

#endif /* !CSCRIPT_POINTER_OBJECT_H */
