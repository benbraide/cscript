#pragma once

#ifndef CSCRIPT_ERROR_H
#define CSCRIPT_ERROR_H

#include "../lexer/token.h"
#include "../object/generic_object.h"

namespace cscript{
	namespace common{
		class error{
		public:
			typedef lexer::token::index index_type;
			typedef object::generic::ptr_type object_type;

			enum class type{
				nil,
				return_,
				break_,
				continue_,
			};

			std::nullptr_t set(object_type object);

			std::nullptr_t set(type type, object_type object = nullptr);

			std::nullptr_t set(const std::string &value);

			std::nullptr_t set(const std::string &value, const index_type &index);

			void warn(const std::string &value);

			void clear();

			void report();

			object_type get();

			bool has() const;

			bool is_return() const;

			bool is_break() const;

			bool is_continue() const;

		private:
			type type_ = type::nil;
			object_type object_;
		};
	}
}

#endif /* !CSCRIPT_ERROR_H */
