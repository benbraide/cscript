#pragma once

#ifndef CSCRIPT_OPERATOR_VALUE_NODE_H
#define CSCRIPT_OPERATOR_VALUE_NODE_H

#include "basic_node.h"

namespace cscript{
	namespace storage{
		enum class operator_value_type{
			binary,
			left,
			right,
		};

		struct operator_key{
			operator_value_type value_type;
			lexer::operator_id id;
			std::string value;
			type::generic::ptr_type type;
		};

		bool operator <(const operator_key &left, const operator_key &right);

		bool operator ==(const operator_key &left, const operator_key &right);
	}

	namespace node{
		class operator_value : public basic{
		public:
			typedef storage::operator_value_type value_type;
			typedef storage::operator_key key;

			operator_value(const lexer::token::index &index, const key &key, const std::string &value);

			virtual ~operator_value();

			virtual ptr_type clone() override;

			virtual object::generic *evaluate() override;

			virtual const key &get_key() const;

			virtual const std::string &get_value() const;

		protected:
			virtual std::string print_() const override;

			key key_;
			std::string value_;
		};
	}
}

#endif /* !CSCRIPT_OPERATOR_VALUE_NODE_H */
