#pragma once

#ifndef CSCRIPT_COMMON_TABLE_H
#define CSCRIPT_COMMON_TABLE_H

#include <string>
#include <unordered_map>

namespace cscript{
	namespace common{
		template <class value_type, class key_type = std::string>
		class table{
		public:
			typedef key_type key_type;
			typedef value_type value_type;

			typedef std::unordered_map<key_type, value_type> map_type;
			typedef typename map_type::const_iterator iterator_type;

		protected:
			iterator_type look_up_(const key_type &key) const{
				return map_.find(key);
			}

			map_type map_;
		};
	}
}

#endif /* !CSCRIPT_COMMON_TABLE_H */
