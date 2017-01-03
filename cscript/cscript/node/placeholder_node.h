#pragma once

#ifndef CSCRIPT_PLACEHOLDER_NODE_H
#define CSCRIPT_PLACEHOLDER_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class placeholder : public basic{
		public:
			placeholder(const lexer::token::index &index, ptr_type value);

			virtual ~placeholder();

			virtual ptr_type clone() override;

			virtual bool is(id id) const override;

			virtual object::generic *evaluate() override;

			virtual std::string get_key() override;

			virtual ptr_type get_value() const;

		protected:
			virtual std::string print_() const override;

			ptr_type value_;
		};
	}
}

#endif /* !CSCRIPT_PLACEHOLDER_NODE_H */
