#pragma once

#ifndef CSCRIPT_RULE_H
#define CSCRIPT_RULE_H

#include <vector>

#include "generic_rule.h"

namespace cscript{
	namespace lexer{
		class rule : public generic_rule{
		public:
			typedef std::vector<token::adjustment> adjustment_list_type;
			typedef std::vector<token_id> list_type;
			typedef list_type::size_type size_type;

			rule();

			virtual bool match(const std::string &value, match_info &matched) const override;

			virtual bool match(iterator_type start, iterator_type end, match_info &matched) const override;

			virtual bool match(const char *start, const char *end, match_info &matched) const override;

			virtual int get_error_index() const override;

			virtual token_id map_index(int index) const override;

			virtual const regex_type &get_compiled() const override;
			
		private:
			template <typename result_type>
			int get_match_index_(result_type &results) const{
				auto index = 0;
				for (auto match = std::next(results.begin()); match != results.end(); ++match, ++index){
					if (match->matched)
						break;
				}

				return index;
			}

			static const list_type map_;

			regex_type compiled_;
		};
	}
}

#endif /* !CSCRIPT_RULE_H */
