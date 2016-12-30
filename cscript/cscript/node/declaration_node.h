#pragma once

#ifndef CSCRIPT_DECLARATION_NODE_H
#define CSCRIPT_DECLARATION_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class declaration : public basic{
		public:
			declaration(const lexer::token::index &index, ptr_type type, ptr_type id);

			virtual ~declaration();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual ptr_type get_type() const;

			virtual ptr_type get_identifier() const;

			virtual memory::virtual_address::attribute get_type_attributes() const;

		protected:
			virtual std::string print_() const override;

			ptr_type type_;
			ptr_type id_;
		};
	}
}

#endif /* !CSCRIPT_DECLARATION_NODE_H */
