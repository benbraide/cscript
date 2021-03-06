#pragma once

#ifndef CSCRIPT_BINARY_OPERATOR_NODE_H
#define CSCRIPT_BINARY_OPERATOR_NODE_H

#include "basic_node.h"
#include "../type/primitive_type.h"

namespace cscript{
	namespace node{
		class binary_operator : public basic{
		public:
			typedef object::generic::binary_info info_type;

			binary_operator(const lexer::token::index &index, const info_type &info, ptr_type left, ptr_type right);

			virtual ~binary_operator();

			virtual ptr_type clone() override;

			virtual bool is(id id) const override;

			virtual object::generic *evaluate() override;

			virtual storage::generic *get_storage() override;

			virtual type::generic::ptr_type get_type() override;

			const info_type &get_info() const;

			ptr_type get_left() const;

			ptr_type get_right() const;

		protected:
			virtual object::generic *short_circuit_(object::generic *left, bool truth, type::boolean_value_type value);

			virtual object::generic *explicit_compare_(object::generic *left, bool truth);

			virtual std::string print_() const override;

			virtual std::string print_open_() const;

			virtual std::string print_close_() const;

			info_type info_;
			ptr_type left_;
			ptr_type right_;
		};
	}
}

#endif /* !CSCRIPT_BINARY_OPERATOR_NODE_H */
