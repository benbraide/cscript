#pragma once

#ifndef CSCRIPT_OPERATOR_SYMBOL_H
#define CSCRIPT_OPERATOR_SYMBOL_H

#include <vector>
#include <boost/regex.hpp>

#include "token_id.h"
#include "token.h"

namespace cscript{
	namespace lexer{
		enum class operator_id{
			nil,
			comma,
			assignment,
			compound_bitwise_or,
			compound_bitwise_xor,
			compound_bitwise_and,
			compound_left_shift,
			compound_right_shift,
			compound_plus,
			compound_minus,
			compound_times,
			compound_divide,
			compound_modulus,
			ternary,
			relational_or,
			relational_and,
			unknown,
			ellipsis,
			bitwise_or,
			bitwise_xor,
			bitwise_and,
			equality,
			explicit_equality,
			inverse_equality,
			explicit_inverse_equality,
			less,
			less_or_equal,
			more_or_equal,
			more,
			left_shift,
			right_shift,
			plus,
			minus,
			times,
			divide,
			modulus,
			new_,
			delete_,
			sizeof_,
			typeof_,
			bitwise_inverse,
			relational_not,
			increment,
			decrement,
			call,
			index,
			member_access,
			member_pointer_access,
			typename_,
			scope_resolution,
		};

		class operator_symbol{
		public:
			typedef std::vector<operator_id> list_type;
			typedef list_type::size_type size_type;

			operator_symbol();

			operator_id match(const std::string &value) const;

			operator_id convert_id(const std::string &value, token_id id) const;

		private:
			int get_match_index_(boost::match_results<std::string::const_iterator> &results) const;

			static const list_type map_;

			boost::regex compiled_;
		};

		class operator_token : public token{
		public:
			operator_token(const index &index, const std::string &value, operator_id id);

			operator_token(const index &index, std::string &&value, operator_id id);

			operator_id get_id() const;

		private:
			operator_id id_;
		};
	}
}

#endif /* !CSCRIPT_OPERATOR_SYMBOL_H */
