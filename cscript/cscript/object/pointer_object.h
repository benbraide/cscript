#pragma once

#ifndef CSCRIPT_POINTER_OBJECT_H
#define CSCRIPT_POINTER_OBJECT_H

#include "numeric_object.h"

namespace cscript{
	namespace object{
		class pointer : public basic{
		public:
			typedef memory::virtual_address::value_type value_type;

			pointer();

			explicit pointer(bool);

			explicit pointer(const type::generic::ptr_type type);

			explicit pointer(value_type value);

			pointer(value_type value, type::generic::ptr_type type);

			pointer(memory::virtual_address::base_type base, const type::generic::ptr_type type);

			virtual ~pointer();

			virtual generic *clone() override;

			virtual generic *cast(const type::generic *type) override;

			virtual generic *evaluate(const binary_info &info) override;

			virtual generic *evaluate(const unary_info &info) override;

			virtual bool to_bool() override;

			virtual std::string to_string() override;

			virtual std::string echo() override;

			virtual bool is_constant() override;

			virtual bool is_constant_target();

			virtual bool is_null();

			virtual bool is_string();

			template <typename value_type>
			value_type get_value(){
				return static_cast<value_type>(get_value_());
			}

			template <typename value_type>
			value_type get_integral_value(){
				if (!is_string())
					return get_value<value_type>();

				try{
					return static_cast<value_type>(std::stoll(get_string_()));
				}
				catch (...){}

				return value_type();
			}

			template <typename value_type>
			value_type get_real_value(){
				if (!is_string())
					return get_value<value_type>();

				try{
					return static_cast<value_type>(std::stold(get_string_()));
				}
				catch (...){}

				return value_type();
			}

			static primitive::boolean::value_type boolean_value(bool value);

		protected:
			virtual void pre_assignment_(generic &operand) override;

			virtual generic *post_assignment_(generic &operand) override;

			virtual generic *create_boolean_(primitive::boolean::value_type value);

			virtual int compare_(generic &operand);

			virtual generic *offset_(bool increment);

			virtual value_type get_value_();

			virtual std::string &get_string_();
		};

		class pointer_ref : public pointer{
		public:
			pointer_ref(memory::virtual_address::value_type memory_value, const type::generic::ptr_type type, bool is_constant);

			virtual ~pointer_ref();

			virtual memory::virtual_address::entry &get_memory() override;

			virtual bool is_lvalue() override;

		protected:
			memory::virtual_address::entry memory_;
		};
	}
}

#endif /* !CSCRIPT_POINTER_OBJECT_H */
