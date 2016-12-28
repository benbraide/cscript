#pragma once

#ifndef CSCRIPT_UNARY_OPERATOR_NODE_H
#define CSCRIPT_UNARY_OPERATOR_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class unary_operator : public basic{
		public:
			typedef object::generic::unary_info info_type;

			unary_operator(const lexer::token::index &index, const info_type &info, ptr_type operand, generic *parent = nullptr);

			virtual ~unary_operator();

			virtual object::generic *evaluate() override;

			const info_type &get_info() const;

			ptr_type get_operand() const;

		protected:
			virtual std::string print_() const override;

			virtual std::string print_open_() const;

			virtual std::string print_close_() const;

			info_type info_;
			ptr_type operand_;
		};
	}
}

#endif /* !CSCRIPT_UNARY_OPERATOR_NODE_H */
