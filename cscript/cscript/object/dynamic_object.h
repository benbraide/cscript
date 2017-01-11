#pragma once

#ifndef CSCRIPT_DYNAMIC_OBJECT_H
#define CSCRIPT_DYNAMIC_OBJECT_H

#include "basic_object.h"

namespace cscript{
	namespace object{
		class dynamic : public basic{
		public:
			typedef memory::virtual_address::value_type value_type;

			explicit dynamic(bool);

			explicit dynamic(const type::generic::ptr_type type);

			explicit dynamic(ptr_type value, const type::generic::ptr_type type);

			dynamic(memory::virtual_address::base_type base, const type::generic::ptr_type type);

			virtual ~dynamic();

			virtual generic *clone() override;

			virtual generic *cast(const type::generic *type) override;

			virtual generic *ref_cast(const type::generic *type) override;

			virtual generic *const_ref_cast(const type::generic *type) override;

			virtual generic *evaluate(const binary_info &info) override;

			virtual generic *evaluate(const unary_info &info) override;

			virtual bool to_bool() override;

			virtual std::string to_string() override;

			virtual std::string echo() override;

			virtual type::generic::ptr_type get_type() override;

			virtual bool is_uninitialized() override;

		protected:
			ptr_type object_;
		};

		class dynamic_ref : public dynamic{
		public:
			dynamic_ref(memory::virtual_address::value_type memory_value, const type::generic::ptr_type type, bool is_constant);

			virtual ~dynamic_ref();

			virtual memory::virtual_address::entry &get_memory() override;

			virtual bool is_lvalue() override;

		protected:
			memory::virtual_address::entry memory_;
		};
	}
}

#endif /* !CSCRIPT_DYNAMIC_OBJECT_H */
