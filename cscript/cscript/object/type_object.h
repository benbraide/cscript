#pragma once

#ifndef CSCRIPT_TYPE_OBJECT_H
#define CSCRIPT_TYPE_OBJECT_H

#include "boolean_object.h"

namespace cscript{
	namespace object{
		namespace primitive{
			class type_object : public basic{
			public:
				typedef memory::virtual_address::value_type value_type;

				type_object();

				explicit type_object(bool);

				explicit type_object(type::generic::ptr_type value);

				explicit type_object(value_type value);

				explicit type_object(memory::virtual_address::base_type base);

				virtual ~type_object();

				virtual generic *clone() override;

				virtual generic *evaluate(const binary_info &info) override;

				virtual generic *evaluate(const unary_info &info) override;

				virtual std::string echo() override;

				virtual type::generic::ptr_type get_type_value() const;

				template <typename value_type>
				value_type get_value(){
					return (value_type)get_value_();
				}

			protected:
				virtual generic *post_assignment_(generic &operand) override;

				virtual value_type get_value_();

				virtual boolean::value_type compare_(generic &operand, bool equality);

				type::generic::ptr_type value_;
			};

			class type_object_ref : public type_object{
			public:
				type_object_ref(memory::virtual_address::value_type memory_value, bool is_constant);

				virtual ~type_object_ref();

				virtual memory::virtual_address::entry &get_memory() override;

				virtual bool is_lvalue() override;

			protected:
				memory::virtual_address::entry memory_;
			};
		}
	}
}

#endif /* !CSCRIPT_TYPE_OBJECT_H */
