#pragma once

#ifndef CSCRIPT_STATIC_ARRAY_NODE_H
#define CSCRIPT_STATIC_ARRAY_NODE_H

#include "declaration_node.h"

namespace cscript{
	namespace node{
		class static_array : public basic{
		public:
			static_array(const lexer::token::index &index, ptr_type declaration, ptr_type size);

			virtual ~static_array();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual ptr_type get_declaration() const;

			virtual ptr_type get_size() const;

			virtual ptr_type get_type() const;

			virtual ptr_type get_identifier() const;

			virtual memory::virtual_address::attribute get_type_attributes() const;

			virtual type::generic::ptr_type get_type_value();

			virtual bool is_static() const;

		protected:
			virtual std::string print_() const override;

			ptr_type declaration_;
			ptr_type size_;
			type::generic::ptr_type type_value_;
		};
	}
}

#endif /* !CSCRIPT_STATIC_ARRAY_NODE_H */
