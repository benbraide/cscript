#pragma once

#ifndef CSCRIPT_DECL_TYPE_NODE_H
#define CSCRIPT_DECL_TYPE_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class decl_type : public basic{
		public:
			decl_type(const lexer::token::index &index, ptr_type value);

			virtual ~decl_type();

			virtual ptr_type clone() override;

			virtual bool is(id id) const override;

			virtual object::generic *evaluate() override;

			virtual storage::generic *get_storage() override;

			virtual type::generic::ptr_type get_type() override;

			virtual ptr_type get_value() const;

		protected:
			virtual std::string print_() const override;

			ptr_type value_;
		};
	}
}

#endif /* !CSCRIPT_DECL_TYPE_NODE_H */
