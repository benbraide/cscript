#pragma once

#ifndef CSCRIPT_PARSER_CONTEXT_H
#define CSCRIPT_PARSER_CONTEXT_H

#include "generic_context.h"

namespace cscript{
	namespace parser{
		namespace context{
			class basic : public generic_context{
			public:
				explicit basic(generic_context *parent);

				virtual ~basic();

				virtual generic_context *get_parent() override;

				virtual bool is_block() const override;

			protected:
				generic_context *parent_;
			};

			class expression : public basic{
			public:
				explicit expression(generic_context *parent = nullptr);

				explicit expression(int precedence, generic_context *parent = nullptr);

				virtual bool accepts(node::id id) const override;

				virtual int get_precedence() const;

				virtual bool is_stackable(int precedence, bool left_associative);

			protected:
				int precedence_;
			};
		}
	}
}

#endif /* !CSCRIPT_PARSER_CONTEXT_H */
