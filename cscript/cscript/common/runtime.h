#pragma once

#ifndef CSCRIPT_RUNTIME_H
#define CSCRIPT_RUNTIME_H

#include "../storage/generic_storage.h"
#include "../type/generic_type.h"

namespace cscript{
	namespace common{
		struct runtime{
			storage::generic *storage;
			type::generic *type;
		};
	}
}

#endif /* !CSCRIPT_RUNTIME_H */
