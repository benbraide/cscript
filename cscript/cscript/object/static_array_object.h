#pragma once

#ifndef CSCRIPT_STATIC_ARRAY_OBJECT_H
#define CSCRIPT_STATIC_ARRAY_OBJECT_H

#include <vector>

#include "pointer_object.h"
#include "../storage/empty_storage.h"

namespace cscript{
	namespace object{
		namespace primitive{
			class static_array : public basic, public storage::empty{
			public:
				typedef std::vector<ptr_type> list_type;

				static_array();

				explicit static_array(const type::generic::ptr_type type);

				static_array(memory::virtual_address::base_type base, const type::generic::ptr_type type);

				virtual ~static_array();

				virtual object::generic *cast(const type::generic *type) override;

				virtual object::generic *evaluate(const binary_info &info) override;

				virtual object::generic *evaluate(const unary_info &info) override;

				virtual std::string echo() override;

				virtual bool is_constant() override;

				virtual bool is_constant_target();

				virtual const list_type &get_entries() const;

			protected:
				virtual void pre_assignment_(object::generic &operand) override;

				virtual std::nullptr_t allocate_(memory::virtual_address::base_type base, const type::generic::ptr_type type);

				list_type entries_;
			};

			class static_array_ref : public static_array{
			public:
				static_array_ref(memory::virtual_address::value_type memory_value, const type::generic::ptr_type type, bool is_constant);

				virtual ~static_array_ref();

				virtual memory::virtual_address::entry &get_memory() override;

				virtual bool is_lvalue() override;

			protected:
				memory::virtual_address::entry memory_;
			};
		}
	}
}

#endif /* !CSCRIPT_STATIC_ARRAY_OBJECT_H */
