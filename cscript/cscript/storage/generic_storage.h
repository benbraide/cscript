#pragma once

#ifndef CSCRIPT_GENERIC_STORAGE_H
#define CSCRIPT_GENERIC_STORAGE_H

#include <string>
#include <functional>

namespace cscript{
	namespace storage{
		class generic_value;
		struct operator_key;

		class generic{
		public:
			struct traversed_key{
				const std::string *string_value;
				const operator_key *operator_value;
			};

			typedef std::function<bool(const traversed_key &, generic_value &)> traverser_type;

			virtual ~generic(){}

			virtual generic *get_parent() = 0;

			virtual generic *get_matched(const std::string &key) = 0;

			virtual generic_value *find(const std::string &key) = 0;

			virtual generic_value *find(const operator_key &key) = 0;

			virtual generic_value &add(const std::string &key) = 0;

			virtual generic_value &add(const operator_key &key) = 0;

			virtual bool remove(const std::string &key) = 0;

			virtual bool remove(const operator_key &key) = 0;

			virtual bool remove(generic_value &value) = 0;

			virtual generic &use(const std::string &key, generic_value &value) = 0;

			virtual generic &use(const operator_key &key, generic_value &value) = 0;

			virtual generic &use(generic &storage) = 0;

			virtual generic &traverse(traverser_type traverser) = 0;

			template <typename object_type>
			object_type *query(){
				return dynamic_cast<object_type *>(this);
			}
		};
	}
}

#endif /* !CSCRIPT_GENERIC_STORAGE_H */
