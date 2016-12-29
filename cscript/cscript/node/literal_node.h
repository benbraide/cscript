#pragma once

#ifndef CSCRIPT_LITERAL_NODE_H
#define CSCRIPT_LITERAL_NODE_H

#include <functional>

#include "basic_node.h"

#include "../type/type_id.h"

namespace cscript{
	namespace node{
		class literal : public basic{
		public:
			typedef std::function<object::generic::ptr_type(const std::string &)> creator;

			struct info{
				object::generic::ptr_type value;
				std::string error;
			};

			literal(const lexer::token::index &index, lexer::token_id id, const std::string &value, creator creator);

			virtual ~literal();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual lexer::token_id get_token_id() const;

		protected:
			virtual std::string print_() const override;

			info info_;
			lexer::token_id id_;
			creator creator_;
			std::string value_;
		};
	}
}

#endif /* !CSCRIPT_LITERAL_NODE_H */
