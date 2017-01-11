#pragma once

#ifndef CSCRIPT_CONTROL_NODE_H
#define CSCRIPT_CONTROL_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace node{
		class control : public basic{
		public:
			enum class option : unsigned int{
				nil				= (0 << 0x0000),
				iterate			= (1 << 0x0000),
				skip_first		= (1 << 0x0001),
				invert			= (1 << 0x0002),
				value			= (1 << 0x0003),
			};

			control(id id, const lexer::token::index &index, option options);

			virtual ~control();

			virtual bool is(id id) const override;

			virtual object::generic *evaluate() override;

			virtual option get_options() const;

		protected:
			virtual object::generic *else_();

			virtual std::nullptr_t init_();

			virtual std::nullptr_t update_();

			virtual bool truth_();

			virtual ptr_type get_predicate_();

			virtual ptr_type get_body_();

			option options_;
		};

		CSCRIPT_MAKE_OPERATORS(control::option)
	}
}

#endif /* !CSCRIPT_CONTROL_NODE_H */
