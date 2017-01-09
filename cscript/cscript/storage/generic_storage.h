#pragma once

#ifndef CSCRIPT_GENERIC_STORAGE_H
#define CSCRIPT_GENERIC_STORAGE_H

#include <string>
#include <functional>

namespace cscript{
	namespace function{
		class generic;
	}

	namespace storage{
		class generic_value;

		class generic{
		public:
			typedef std::function<bool(const std::string &, generic_value &)> traverser_type;
			typedef std::shared_ptr<cscript::function::generic> function_type;

			virtual ~generic(){}

			virtual void store_function(function_type object) = 0;

			virtual generic *get_parent() = 0;

			virtual generic *get_matched(const std::string &key) = 0;

			virtual generic_value *find(const std::string &key) = 0;

			virtual generic_value &add(const std::string &key) = 0;

			virtual bool remove(const std::string &key) = 0;

			virtual bool remove(generic_value &value) = 0;

			virtual generic &use(const std::string &key, generic_value &value) = 0;

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
