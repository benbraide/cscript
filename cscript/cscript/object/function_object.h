#pragma once

#ifndef CSCRIPT_FUNCTION_OBJECT_H
#define CSCRIPT_FUNCTION_OBJECT_H

#include "boolean_object.h"
#include "../function/generic_function.h"

namespace cscript{
	namespace object{
		namespace primitive{
			class function_object : public basic{
			public:
				typedef memory::virtual_address::value_type value_type;

				explicit function_object(const type::generic::ptr_type type);

				explicit function_object(bool);

				explicit function_object(value_type value);

				function_object(memory::virtual_address::base_type base, const type::generic::ptr_type type);

				virtual ~function_object();

				virtual generic *clone() override;

				virtual generic *evaluate(const binary_info &info) override;

				virtual std::string echo() override;

				virtual function::generic *get_function_value();

			protected:
				virtual value_type get_value_();

				virtual boolean::value_type compare_(generic &operand, bool equality);
			};

			class function_object_ref : public function_object{
			public:
				function_object_ref(memory::virtual_address::value_type memory_value, const type::generic::ptr_type type, bool is_constant);

				virtual ~function_object_ref();

				virtual memory::virtual_address::entry &get_memory() override;

				virtual bool is_lvalue() override;

			protected:
				memory::virtual_address::entry memory_;
			};
		}
	}
}

#endif /* !CSCRIPT_FUNCTION_OBJECT_H */
