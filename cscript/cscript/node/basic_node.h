#pragma once

#ifndef CSCRIPT_BASIC_NODE_H
#define CSCRIPT_BASIC_NODE_H

#include "generic_node.h"

namespace cscript{
	namespace node{
		class basic : public generic{
		public:
			basic(id id, const lexer::token::index &index, generic *parent = nullptr);

			virtual ~basic();

			virtual const generic *get_parent() const override;

			virtual generic *get_parent() override;

			virtual id get_id() const override;

			virtual const lexer::token::index &get_index() const override;

			virtual bool is(id id) const override;

			virtual std::string print() const override;

			virtual int get_print_tab() const override;

		protected:
			virtual std::string print_() const;

			id id_;
			generic *parent_;
			lexer::token::index index_;
		};
	}
}

#endif /* !CSCRIPT_BASIC_NODE_H */
