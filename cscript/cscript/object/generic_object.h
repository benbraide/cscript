#pragma once

#ifndef CSCRIPT_GENERIC_OBJECT_H
#define CSCRIPT_GENERIC_OBJECT_H

#include <string>
#include <memory>

#include "../lexer/operator_symbol.h"
#include "../memory/virtual_address.h"
#include "../type/generic_type.h"

namespace cscript{
	namespace object{
		class generic{
		public:
			typedef std::shared_ptr<generic> ptr_type;

			struct binary_info{
				lexer::operator_id id;
				std::string value;
			};

			struct unary_info{
				bool left;
				lexer::operator_id id;
				std::string value;
			};

			virtual ~generic(){}

			virtual generic *remove_reference() = 0;

			virtual generic *clone() = 0;

			virtual generic *cast(const type::generic *type) = 0;

			virtual generic *ref_cast(const type::generic *type) = 0;

			virtual generic *evaluate(const binary_info &info) = 0;

			virtual generic *evaluate(const unary_info &info) = 0;

			virtual bool to_bool() = 0;

			virtual std::string to_string() = 0;

			virtual std::string echo() = 0;

			virtual memory::virtual_address::entry &get_memory() = 0;

			virtual type::generic::ptr_type get_type() = 0;

			virtual storage::generic *get_storage() = 0;

			virtual bool is_lvalue() const = 0;

			virtual bool is_reference() const = 0;

			virtual bool is_indirect() const = 0;

			virtual bool is_uninitialized() const = 0;

			virtual bool is_constant() const = 0;

			template <typename object_type>
			object_type *query(){
				return dynamic_cast<object_type *>(this);
			}
		};
	}
}

#endif /* !CSCRIPT_GENERIC_OBJECT_H */
