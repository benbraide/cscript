#pragma once

#ifndef CSCRIPT_GENERIC_TYPE_H
#define CSCRIPT_GENERIC_TYPE_H

#include <string>
#include <memory>

#include "type_id.h"
#include "../memory/virtual_address.h"

namespace cscript{
	namespace type{
		class generic{
		public:
			typedef std::shared_ptr<generic> ptr_type;
			typedef memory::pool::size_type size_type;

			virtual ~generic(){}

			virtual const generic *base() const = 0;

			virtual std::string name() const = 0;

			virtual std::string print() const = 0;

			virtual id get_id() const = 0;

			virtual size_type get_size() const = 0;

			virtual size_type get_composite_size(const generic *type) const = 0;

			virtual const generic *get_same(const generic *type) const = 0;

			virtual const generic *get_bully(const generic *type) const = 0;

			virtual int get_score(const generic *type) const = 0;

			virtual bool has_conversion(const generic *type) const = 0;

			virtual bool is_same(const generic *type) const = 0;

			virtual bool is_choice() const = 0;

			virtual bool is_any() const = 0;

			virtual bool is_auto() const = 0;

			virtual bool is_variadic() const = 0;

			virtual bool is_primitive() const = 0;

			virtual bool is_class()const = 0;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_TYPE_H */
