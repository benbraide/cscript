#pragma once

#ifndef CSCRIPT_GENERIC_TYPE_H
#define CSCRIPT_GENERIC_TYPE_H

#include <string>
#include <memory>

#include "type_id.h"
#include "../memory/virtual_address.h"

namespace cscript{
	namespace object{
		class generic;
	}

	namespace type{
		class generic{
		public:
			typedef std::shared_ptr<generic> ptr_type;
			typedef memory::pool::size_type size_type;

			virtual ~generic(){}

			virtual const generic *base() const = 0;

			virtual const generic *remove_pointer() const = 0;

			virtual std::string name() const = 0;

			virtual std::string print() const = 0;

			virtual std::shared_ptr<object::generic> create(ptr_type this_ptr) = 0;

			virtual std::shared_ptr<object::generic> create(memory::virtual_address::base_type base, ptr_type this_ptr) = 0;

			virtual std::shared_ptr<object::generic> create_ref(memory::virtual_address::value_type memory_value,
				bool is_constant, ptr_type this_ptr) = 0;

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

			virtual bool is_class()const = 0;

			virtual bool is_primitive() const = 0;

			virtual bool is_numeric() const = 0;

			virtual bool is_integral() const = 0;

			virtual bool is_unsigned_integral() const = 0;

			virtual bool is_pointer() const = 0;

			template <typename object_type>
			object_type *query(){
				return dynamic_cast<object_type *>(this);
			}

			template <typename object_type>
			const object_type *query() const{
				return dynamic_cast<const object_type *>(this);
			}
		};
	}
}

#endif /* !CSCRIPT_GENERIC_TYPE_H */
