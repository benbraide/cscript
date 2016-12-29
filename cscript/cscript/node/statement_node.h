#pragma once

#ifndef CSCRIPT_STATEMENT_NODE_H
#define CSCRIPT_STATEMENT_NODE_H

#include "basic_node.h"

#include "../type/type_id.h"

namespace cscript{
	namespace node{
		class statement : public basic{
		public:
			typedef std::function<object::generic::ptr_type(const std::string &)> creator;

			statement(const lexer::token::index &index, ptr_type collection);

			virtual ~statement();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual ptr_type get_collection();

		protected:
			ptr_type collection_;
		};
	}
}

#endif /* !CSCRIPT_STATEMENT_NODE_H */
