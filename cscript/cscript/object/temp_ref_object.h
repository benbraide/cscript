#pragma once

#ifndef CSCRIPT_TEMP_REF_OBJECT_H
#define CSCRIPT_TEMP_REF_OBJECT_H

#include "basic_object.h"

namespace cscript{
	namespace object{
		class temp_ref : public basic{
		public:
			typedef memory::virtual_address::entry value_type;

			explicit temp_ref(value_type &value, bool is_constant = false);

			virtual ~temp_ref();

			virtual generic *clone() override;

			virtual generic *cast(const type::generic *type) override;

			virtual generic *ref_cast(const type::generic *type) override;

			virtual generic *evaluate(const binary_info &info) override;

			virtual generic *evaluate(const unary_info &info) override;

			virtual bool to_bool() override;

			virtual memory::virtual_address::entry &get_memory() override;

			virtual type::generic::ptr_type get_type() override;

			virtual storage::generic *get_storage() override;

			virtual bool is_lvalue() const override;

			virtual bool is_reference() const override;

			virtual bool is_indirect() const override;

			virtual bool is_uninitialized() const override;

			virtual bool is_constant() const override;

		protected:
			value_type &value_;
			bool is_constant_;
		};
	}
}

#endif /* !CSCRIPT_TEMP_REF_OBJECT_H */
