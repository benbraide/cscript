#pragma once

#ifndef CSCRIPT_GENERIC_FUNCTION_H
#define CSCRIPT_GENERIC_FUNCTION_H

#include "../object/generic_object.h"
#include "../storage/generic_storage.h"

namespace cscript{
	namespace function{
		class definition;

		class generic{
		public:
			virtual ~generic(){}

			virtual object::generic *call() = 0;

			virtual generic &add(generic &value) = 0;

			virtual generic *get_matched(int &score) = 0;

			virtual generic *get_matched(const type::generic *type) = 0;

			virtual type::generic::ptr_type get_type() = 0;

			virtual definition *get_definition() = 0;

			virtual storage::generic *get_storage() = 0;

			virtual const std::string &get_name() const = 0;

			virtual std::string print() = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_FUNCTION_H */
