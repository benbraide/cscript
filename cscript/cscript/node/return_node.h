#pragma once

#ifndef CSCRIPT_RETURN_NODE_H
#define CSCRIPT_RETURN_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class return_node : public basic{
		public:
			return_node(const lexer::token::index &index, ptr_type value);

			virtual ~return_node();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual ptr_type get_value() const;

		protected:
			virtual std::string print_() const override;

			ptr_type value_;
		};
	}
}

#endif /* !CSCRIPT_RETURN_NODE_H */
