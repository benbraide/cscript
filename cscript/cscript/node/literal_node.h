#pragma once

#ifndef CSCRIPT_LITERAL_NODE_H
#define CSCRIPT_LITERAL_NODE_H

#include <functional>

#include "basic_node.h"

#include "../type/type_id.h"
#include "../common/env.h"
#include "../object/primitive_object.h"

namespace cscript{
	namespace node{
		class literal : public basic{
		public:
			typedef std::function<object::generic::ptr_type(const std::string &)> creator;

			literal(const lexer::token::index &index, const std::string &value, const std::string &suffix, creator creator);

			virtual ~literal();

			virtual object::generic::ptr_type evaluate() override;

		protected:
			lexer::token::index index_;
			std::string value_;
			std::string suffix_;
			creator creator_;
		};
	}
}

#endif /* !CSCRIPT_LITERAL_NODE_H */
