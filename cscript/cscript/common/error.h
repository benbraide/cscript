#pragma once

#ifndef CSCRIPT_ERROR_H
#define CSCRIPT_ERROR_H

#include <string>

namespace cscript{
	namespace common{
		class error{
		public:
			struct info{
				std::string value;
				int line;
				int column;
			};

			nullptr_t set(const std::string &value);

			template <typename index_type>
			nullptr_t set(const std::string &value, const index_type &index){
				return nullptr;
			}

			template <typename index_type>
			void set_warning(const std::string &value, const index_type &index){

			}

			bool has() const;
		};
	}
}

#endif /* !CSCRIPT_ERROR_H */
