#pragma once

#ifndef CSCRIPT_TYPEDEF_NODE_H
#define CSCRIPT_TYPEDEF_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class type_definition : public basic{
		public:
			type_definition(const lexer::token::index &index, ptr_type type, ptr_type name);

			virtual ~type_definition();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual ptr_type get_type() const;

			virtual ptr_type get_name() const;

		protected:
			virtual std::string print_() const override;

			ptr_type type_;
			ptr_type name_;
		};
	}
}

#endif /* !CSCRIPT_TYPEDEF_NODE_H */
