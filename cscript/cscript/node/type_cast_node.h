#pragma once

#ifndef CSCRIPT_TYPE_CAST_NODE_H
#define CSCRIPT_TYPE_CAST_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class type_cast : public basic{
		public:
			type_cast(const lexer::token::index &index, ptr_type type, ptr_type value);

			virtual ~type_cast();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual ptr_type get_type() const;

			virtual ptr_type get_value() const;

		protected:
			virtual std::string print_() const override;

			ptr_type type_;
			ptr_type value_;
		};
	}
}

#endif /* !CSCRIPT_TYPE_CAST_NODE_H */
