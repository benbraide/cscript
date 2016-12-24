#pragma once

#ifndef CSCRIPT_BASIC_OBJECT_H
#define CSCRIPT_BASIC_OBJECT_H

#include "generic_object.h"

#include "../common/env.h"

namespace cscript{
	namespace object{
		class basic : public generic{
		public:
			explicit basic(memory::virtual_address::entry &memory);

			virtual ~basic();

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
			memory::virtual_address::entry &memory_;
		};
	}
}

#endif /* !CSCRIPT_BASIC_OBJECT_H */
