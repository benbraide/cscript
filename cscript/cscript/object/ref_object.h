#pragma once

#ifndef CSCRIPT_REF_OBJECT_H
#define CSCRIPT_REF_OBJECT_H

#include "basic_object.h"

namespace cscript{
	namespace object{
		class ref : public basic{
		public:
			typedef memory::virtual_address::entry value_type;

			explicit ref(value_type &value, bool is_constant = false);

			virtual ~ref();

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

#endif /* !CSCRIPT_REF_OBJECT_H */
