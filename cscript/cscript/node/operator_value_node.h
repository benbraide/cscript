#pragma once

#ifndef CSCRIPT_OPERATOR_VALUE_NODE_H
#define CSCRIPT_OPERATOR_VALUE_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class operator_value : public basic{
		public:
			struct key{
				bool right;
				std::string value;
				ptr_type type;
			};

			operator_value(const lexer::token::index &index, const key &key);

			virtual ~operator_value();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual std::string get_key() override;

			virtual const key &get_key_value() const;

		protected:
			virtual std::string print_() const override;

			key key_;
			std::string string_key_;
		};
	}
}

#endif /* !CSCRIPT_OPERATOR_VALUE_NODE_H */
