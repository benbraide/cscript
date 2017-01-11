#pragma once

#ifndef CSCRIPT_ITERATION_NODE_H
#define CSCRIPT_ITERATION_NODE_H

#include "selection_node.h"

namespace cscript{
	namespace node{
		class for_iteration : public control{
		public:
			for_iteration(const lexer::token::index &index, ptr_type init, ptr_type predicate, ptr_type body,
				ptr_type update, ptr_type else_node = nullptr);

			virtual ~for_iteration();

			virtual ptr_type clone() override;

			virtual ptr_type get_init();

			virtual ptr_type get_predicate();

			virtual ptr_type get_body();

			virtual ptr_type get_update();

			virtual ptr_type get_else_node();

		protected:
			virtual std::string print_() const override;

			virtual object::generic *else_() override;

			virtual std::nullptr_t init_() override;

			virtual std::nullptr_t update_() override;

			virtual ptr_type get_predicate_() override;

			virtual ptr_type get_body_() override;

			ptr_type init_node_;
			ptr_type predicate_;
			ptr_type body_;
			ptr_type update_node_;
			ptr_type else_node_;
		};

		template <class env_type>
		using while_iteration = selection<env_type, id::while_, control::option::iterate>;

		template <class env_type>
		using until_iteration = selection<env_type, id::until, control::option::iterate | control::option::invert>;

		template <class env_type>
		using do_while_iteration = selection<env_type, id::until, control::option::iterate | control::option::skip_first>;
	}
}

#endif /* !CSCRIPT_ITERATION_NODE_H */
