#pragma once

#ifndef CSCRIPT_COMMON_TABLE_H
#define CSCRIPT_COMMON_TABLE_H

#include <map>
#include <list>
#include <string>
#include <vector>
#include <boost/regex.hpp>

namespace cscript{
	namespace common{
		template <class value_type>
		class table{
		public:
			typedef value_type value_type;
			typedef std::map<std::string, value_type> map_type;

			typedef typename map_type::const_iterator iterator_type;
			typedef typename map_type::size_type size_type;

			typedef std::string string_type;
			typedef std::list<string_type> string_list_type;

			typedef boost::regex regex_type;
			typedef std::list<regex_type> regex_list_type;

		protected:
			void compile_(){
				string_type compiled_value;

				auto max_size = compiled_value.max_size();
				auto regex = &compiled_;

				others_.clear();
				for (auto &entry : map_){
					if ((max_size - compiled_value.size()) < entry.first.size()){
						regex->assign("^" + compiled_value + "$", boost::regex_constants::optimize);
						regex = &*others_.emplace(others_.end());
						compiled_value.clear();
					}
					
					if (compiled_value.empty())
						compiled_value = "(" + entry.first + ")";
					else//Append
						compiled_value += "|(" + entry.first + ")";
				}

				if (!compiled_value.empty())
					regex->assign("^" + compiled_value + "$", boost::regex_constants::optimize);
			}

			iterator_type look_up_(const std::string &name) const{
				if (map_.empty())
					return map_.end();

				auto value = look_up_(name, compiled_);
				if (value != map_.end())
					return value;

				for (auto &regex : others_){
					if ((value = look_up_(name, regex)) != map_.end())
						return value;
				}

				return map_.end();
			}

			iterator_type look_up_(const std::string &name, const regex_type &regex) const{
				boost::match_results<std::string::const_iterator> results;
				if (!boost::regex_search(name.begin(), name.end(), results, regex, boost::regex_constants::match_single_line))
					return map_.end();

				auto index = get_match_index_(results);
				return (index < map_.size()) ? std::next(map_.begin(), index) : map_.end();
			}

			size_type get_match_index_(boost::match_results<std::string::const_iterator> &results) const{
				size_type index = 0;
				for (auto match = std::next(results.begin()); match != results.end(); ++match, ++index){
					if (match->matched)
						break;
				}

				return index;
			}

			map_type map_;
			regex_type compiled_;
			regex_list_type others_;
		};
	}
}

#endif /* !CSCRIPT_COMMON_TABLE_H */
