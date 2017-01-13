#pragma once

#ifndef CSCRIPT_RUNTIME_H
#define CSCRIPT_RUNTIME_H

#include <list>

#include "../storage/generic_storage.h"
#include "../type/generic_type.h"
#include "../object/generic_object.h"
#include "../node/generic_node.h"
#include "../lexer/token.h"

namespace cscript{
	namespace common{
		struct declaration_info{
			storage::generic_value *value;
			object::generic::ptr_type object;
			memory::virtual_address::attribute attributes;
		};

		struct operand_info{
			node::generic *node;
			object::generic *object;
			std::list<object::generic *> constant_objects;
		};

		struct runtime{
			storage::generic *storage;
			type::generic *type;
			char *base;
			declaration_info declaration;
			operand_info operand;
			std::list<object::generic *> arguments;
			lexer::token::index index;
		};
	}
}

#endif /* !CSCRIPT_RUNTIME_H */
