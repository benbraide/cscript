#pragma once

#ifndef CSCRIPT_TYPE_NODE_H
#define CSCRIPT_TYPE_NODE_H

#include "basic_node.h"
#include "../memory/virtual_address.h"

namespace cscript{
	namespace node{
		class primitive_type : public basic{
		public:
			primitive_type(const lexer::token::index &index, lexer::token_id value);

			virtual ~primitive_type();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual std::string get_key() override;

			virtual storage::generic *get_storage() override;

			virtual type::generic::ptr_type get_type() override;

			virtual lexer::token_id get_value() const;

		protected:
			virtual std::string print_() const override;

			lexer::token_id value_;
		};

		class type_with_storage_class : public basic{
		public:
			typedef memory::virtual_address::attribute attribute_type;

			type_with_storage_class(const lexer::token::index &index, attribute_type attributes, ptr_type type, const std::string &value);

			virtual ~type_with_storage_class();

			virtual ptr_type clone() override;

			virtual bool is(id id) const override;

			virtual object::generic *evaluate() override;

			virtual std::string get_key() override;

			virtual storage::generic *get_storage() override;

			virtual type::generic::ptr_type get_type() override;

			virtual attribute_type get_attributes() const;

			virtual ptr_type get_value() const;

		protected:
			virtual std::string print_() const override;

			attribute_type attributes_;
			ptr_type type_;
			std::string value_;
		};
	}
}

#endif /* !CSCRIPT_TYPE_NODE_H */
