#pragma once

#ifndef CSCRIPT_BASIC_NODE_H
#define CSCRIPT_BASIC_NODE_H

#include "generic_node.h"

namespace cscript{
	namespace node{
		class basic : public generic{
		public:
			explicit basic(id id, generic *parent = nullptr);

			virtual ~basic();

			virtual const generic *get_parent() const override;

			virtual generic *get_parent() override;

			virtual id get_id() const override;

			virtual bool is(id id) const override;

		protected:
			id id_;
			generic *parent_;
		};
	}
}

#endif /* !CSCRIPT_BASIC_NODE_H */
