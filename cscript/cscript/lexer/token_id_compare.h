#pragma once

#ifndef CSCRIPT_TOKEN_ID_COMPARE_H
#define CSCRIPT_TOKEN_ID_COMPARE_H

#include <vector>

#include "generic_token_id_compare.h"

namespace cscript{
	namespace lexer{
		class token_id_compare : public generic_token_id_compare{
		public:
			typedef std::vector<token_id> list_type;

			explicit token_id_compare(const list_type &list);

			explicit token_id_compare(list_type &&list);

			virtual bool is(token_id value) const override;

		private:
			list_type list_;
		};

		class blank_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class literal_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class numeric_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class integral_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class real_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class string_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class escaped_string_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class keyword_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class context_keyword_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class block_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class storage_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class type_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class control_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class group_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class skip_token_id : public generic_token_id_compare{
		public:
			virtual bool is(token_id value) const override;
		};

		class aggregate : public generic_token_id_compare{
		public:
			typedef std::vector<generic_token_id_compare *> list_type;

			explicit aggregate(const list_type &list);

			explicit aggregate(list_type &&list);

			virtual bool is(token_id value) const override;

		private:
			list_type list_;
		};

		class token_id_compare_collection{
		public:
			static const blank_token_id				blank;
			static const literal_token_id			literal;
			static const numeric_token_id			numeric;
			static const integral_token_id			integral;
			static const real_token_id				real;
			static const string_token_id			string;
			static const escaped_string_token_id	escaped_string;
			static const keyword_token_id			keyword;
			static const context_keyword_token_id	context_keyword;
			static const block_token_id				block;
			static const storage_token_id			storage;
			static const type_token_id				type;
			static const control_token_id			control;
			static const group_token_id				group;
			static const skip_token_id				skip;
		};
	}
}

#endif /* !CSCRIPT_TOKEN_ID_COMPARE_H */
