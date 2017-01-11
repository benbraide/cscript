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

			virtual type::generic::ptr_type get_type_value();

			virtual bool is_static() const;

			virtual object::generic *allocate();

			virtual object::generic *allocate(type::generic::ptr_type type);

			static object::generic *allocate(const lexer::token::index &index, type::generic::ptr_type type,
				ptr_type id, memory::virtual_address::attribute attributes);

		protected:
			virtual std::string print_() const override;

			ptr_type type_;
			ptr_type id_;
			type::generic::ptr_type type_value_;
		};

		class dependent_declaration : public declaration{
		public:
			dependent_declaration(const lexer::token::index &index, ptr_type declaration, ptr_type id);

			virtual ~dependent_declaration();

			virtual ptr_type clone() override;

			virtual object::generic *allocate(type::generic::ptr_type type) override;

			virtual ptr_type get_type() const;

			virtual ptr_type get_declaration() const;

			virtual memory::virtual_address::attribute get_type_attributes() const;

			virtual type::generic::ptr_type get_type_value();

		protected:
			virtual std::string print_() const override;
		};

		class initialization_declaration : public declaration{
		public:
			initialization_declaration(const lexer::token::index &index, ptr_type declaration, ptr_type value);

			virtual ~initialization_declaration();

			virtual ptr_type clone() override;

			virtual const lexer::token::index &get_index() const override;

			virtual object::generic *evaluate() override;

			virtual object::generic *allocate(type::generic::ptr_type type) override;

			virtual ptr_type get_type() const;

			virtual ptr_type get_identifier() const;

			virtual ptr_type get_declaration() const;

			virtual ptr_type get_value() const;

			virtual memory::virtual_address::attribute get_type_attributes() const;

			virtual type::generic::ptr_type get_type_value();

		protected:
			virtual std::string print_() const override;
		};
	}
}

#endif /* !CSCRIPT_DECLARATION_NODE_H */
