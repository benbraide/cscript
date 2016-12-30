#pragma once

#ifndef CSCRIPT_RUNTIME_H
#define CSCRIPT_RUNTIME_H

#include "../storage/generic_storage.h"
#include "../type/generic_type.h"
#include "../object/generic_object.h"

namespace cscript{
	namespace common{
		struct declaration_info{
			storage::generic_value *value;
			object::generic::ptr_type object;
			memory::virtual_address::attribute attributes;
		};

		struct runtime{
			storage::generic *storage;
			type::generic *type;
			declaration_info declaration;
		};
	}
}

#endif /* !CSCRIPT_RUNTIME_H */
