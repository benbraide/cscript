#pragma once

#ifndef CSCRIPT_BYTE_OBJECT_H
#define CSCRIPT_BYTE_OBJECT_H

#include "boolean_object.h"

namespace cscript{
	namespace object{
		namespace primitive{
			class byte : public basic{
			public:
				typedef unsigned char value_type;

				byte();

				explicit byte(bool);

				explicit byte(value_type value);

				explicit byte(memory::virtual_address::base_type base);

				virtual ~byte();

				virtual generic *clone() override;

				virtual generic *cast(const type::generic *type) override;

				virtual generic *evaluate(const binary_info &info) override;

				virtual std::string echo() override;

				template <typename value_type>
				value_type get_value(){
					return (value_type)get_value_();
				}

			protected:
				virtual value_type get_value_();

				virtual boolean::value_type compare_(generic &operand, bool equality);
			};

			class byte_ref : public byte{
			public:
				byte_ref(memory::virtual_address::value_type memory_value, bool is_constant);

				virtual ~byte_ref();

				virtual memory::virtual_address::entry &get_memory() override;

				virtual bool is_lvalue() override;

			protected:
				memory::virtual_address::entry memory_;
			};
		}
	}
}

#endif /* !CSCRIPT_BYTE_OBJECT_H */
