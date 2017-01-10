#pragma once

#ifndef CSCRIPT_FUNCTION_NODE_H
#define CSCRIPT_FUNCTION_NODE_H

#include "basic_node.h"
#include "../function/function_entry.h"

namespace cscript{
	namespace node{
		class function : public basic{
		public:
			typedef cscript::function::entry::parameter_limits parameter_limits;

			function(const lexer::token::index &index, ptr_type declaration, ptr_type parameters, ptr_type definition);

			virtual ~function();

			virtual ptr_type clone() override;

			virtual bool is(id id) const override;

			virtual object::generic *evaluate() override;

			virtual ptr_type get_declaration() const;

			virtual ptr_type get_parameters() const;

			virtual ptr_type get_definition() const;

		protected:
			virtual std::string print_() const override;

			virtual std::nullptr_t insert_(cscript::function::generic *value);

			virtual std::nullptr_t resolve_type_();

			ptr_type declaration_;
			ptr_type parameters_;
			ptr_type definition_;
			type::generic::ptr_type type_value_;
			parameter_limits parameter_limits_;
		};
	}
}

#endif /* !CSCRIPT_FUNCTION_NODE_H */
