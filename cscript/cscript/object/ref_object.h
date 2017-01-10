#pragma once

#ifndef CSCRIPT_REF_OBJECT_H
#define CSCRIPT_REF_OBJECT_H

#include "basic_object.h"

namespace cscript{
	namespace object{
		class ref : public basic{
		public:
			typedef memory::virtual_address::value_type value_type;

			explicit ref(const type::generic::ptr_type type);

			explicit ref(value_type value, const type::generic::ptr_type type, bool is_constant = false);

			ref(memory::virtual_address::base_type base, const type::generic::ptr_type type);

			virtual ~ref();

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

			virtual bool is_reference() override;

			virtual bool is_constant() override;

		protected:
			virtual generic *get_object_();

			virtual value_type get_value_();

			bool is_constant_;
		};
	}
}

#endif /* !CSCRIPT_REF_OBJECT_H */
