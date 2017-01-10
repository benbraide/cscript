#pragma once

#ifndef CSCRIPT_BASIC_OBJECT_H
#define CSCRIPT_BASIC_OBJECT_H

#include "generic_object.h"

#include "../common/env.h"

namespace cscript{
	namespace object{
		class basic : public generic{
		public:
			typedef memory::virtual_address::entry address_entry;
			typedef memory::virtual_address::value_type address_value_type;
			typedef memory::virtual_address::size_type address_size_type;

			explicit basic(memory::virtual_address::value_type memory_value = 0u);

			virtual ~basic();

			virtual ptr_type get_ptr() override;

			virtual generic *remove_reference() override;

			virtual generic *clone() override;

			virtual generic *cast(const type::generic *type) override;

			virtual generic *ref_cast(const type::generic *type) override;

			virtual generic *const_ref_cast(const type::generic *type) override;

			virtual generic *evaluate(const binary_info &info) override;

			virtual generic *evaluate(const unary_info &info) override;

			virtual bool to_bool() override;

			virtual std::string to_string() override;

			virtual std::string echo() override;

			virtual generic &set_memory_value(memory::virtual_address::value_type value) override;

			virtual memory::virtual_address::value_type get_memory_value() override;

			virtual memory::virtual_address::entry &get_memory() override;

			virtual type::generic::ptr_type get_type() override;

			virtual storage::generic *get_storage() override;

			virtual bool is_lvalue() override;

			virtual bool is_reference() override;

			virtual bool is_indirect() override;

			virtual bool is_uninitialized() override;

			virtual bool is_temp() override;

			virtual bool is_constant() override;

		protected:
			virtual void pre_assignment_(generic &operand);

			virtual generic *post_assignment_(generic &operand);

			memory::virtual_address::value_type memory_value_;
		};
	}
}

#endif /* !CSCRIPT_BASIC_OBJECT_H */
