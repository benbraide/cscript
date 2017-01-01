#pragma once

#ifndef CSCRIPT_IDENTIFIER_NODE_H
#define CSCRIPT_IDENTIFIER_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class identifier : public basic{
		public:
			identifier(const lexer::token::index &index, const std::string &value);

			virtual ~identifier();

			virtual ptr_type clone() override;

			virtual bool is(id id) const override;

			virtual object::generic *evaluate() override;

			virtual std::string get_key() override;

			virtual const std::string &get_value() const;

		protected:
			virtual std::string print_() const override;

			std::string value_;
		};
	}
}

#endif /* !CSCRIPT_IDENTIFIER_NODE_H */
