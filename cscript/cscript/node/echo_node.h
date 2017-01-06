#pragma once

#ifndef CSCRIPT_ECHO_NODE_H
#define CSCRIPT_ECHO_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class echo : public basic{
		public:
			echo(const lexer::token::index &index, ptr_type value);

			virtual ~echo();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual ptr_type get_value() const;

		protected:
			virtual std::string print_() const override;

			ptr_type value_;
		};
	}
}

#endif /* !CSCRIPT_ECHO_NODE_H */
