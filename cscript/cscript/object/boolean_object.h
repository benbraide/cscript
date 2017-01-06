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

				explicit boolean(bool);

				explicit boolean(value_type value);

				explicit boolean(memory::virtual_address::base_type base);

				virtual ~boolean();

				virtual generic *clone() override;

				virtual generic *evaluate(const binary_info &info) override;

				virtual bool to_bool() override;

				virtual std::string echo() override;

			protected:
				virtual value_type get_value_();

				virtual value_type compare_(generic &operand, bool equality);
			};

			class boolean_ref : public boolean{
			public:
				boolean_ref(memory::virtual_address::value_type memory_value, bool is_constant);

				virtual ~boolean_ref();

				virtual memory::virtual_address::entry &get_memory() override;

				virtual bool is_lvalue() override;

			protected:
				memory::virtual_address::entry memory_;
			};
		}
	}
}

#endif /* !CSCRIPT_BOOLEAN_OBJECT_H */
