#pragma once

#ifndef CSCRIPT_GENERIC_NODE_H
#define CSCRIPT_GENERIC_NODE_H

#include "node_id.h"

namespace cscript{
	namespace node{
		class generic{
		public:
			virtual ~generic(){}

			virtual const generic *get_parent() const = 0;

			virtual generic *get_parent() = 0;

			virtual id get_id() const = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_NODE_H */
