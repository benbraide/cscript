#pragma once

#ifndef CSCRIPT_ITERATION_NODE_H
#define CSCRIPT_ITERATION_NODE_H

#include "selection_node.h"

namespace cscript{
	namespace node{
		template <class env_type>
		using while_iteration = selection<env_type, id::while_, control::option::iterate>;

		template <class env_type>
		using until_iteration = selection<env_type, id::until, control::option::iterate | control::option::invert>;

		template <class env_type>
		using do_while_iteration = selection<env_type, id::until, control::option::iterate | control::option::skip_first>;
	}
}

#endif /* !CSCRIPT_ITERATION_NODE_H */
