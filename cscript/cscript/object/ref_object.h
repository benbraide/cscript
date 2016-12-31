#pragma once

#ifndef CSCRIPT_REF_OBJECT_H
#define CSCRIPT_REF_OBJECT_H

#include "basic_object.h"

namespace cscript{
	namespace object{
		class ref : public basic{
		public:
			typedef memory::virtual_address::entry entry_type;
			typedef memory::virtual_address::value_type value_type;

			explicit ref(const type::generic::ptr_type type);

			explicit ref(entry_type &value, bool is_constant = false);

			virtual ~ref();

			virtual generic *clone() override;

			virtual generic *cast(const type::generic *type) override;

			virtual generic *ref_cast(const type::generic *type) override;

			virtual generic *evaluate(const binary_info &info) override;

			virtual generic *evaluate(const unary_info &info) override;

			virtual bool to_bool() override;

			virtual bool is_constant_ref() const;

		protected:
			bool is_constant_;
			entry_type *value_;
		};
	}
}

#endif /* !CSCRIPT_REF_OBJECT_H */
