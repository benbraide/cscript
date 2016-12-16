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

		class skippable_token_id : public generic_token_id_compare{
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

		class string_token_id : public generic_token_id_compare{
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
	}
}

#endif /* !CSCRIPT_TOKEN_ID_COMPARE_H */
