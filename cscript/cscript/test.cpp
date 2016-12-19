#include <iostream>

#include "lexer/string_source.h"
#include "lexer/token_formatter.h"
#include "lexer/file_source.h"

using namespace cscript::lexer;
using namespace cscript::lexer::source;

int main(){
	{
		rule rule;
		file ss("test/sample.txt");
		defined_symbols symbols;
		source_guard guard;

		source_info info{
			symbols,
			ss,
			rule,
			guard,
			&token_id_compare_collection::skip,
			&formatter::linked_collection::last
		};

		while (ss.has_more()){
			auto v = ss.next(info);
			if (v == nullptr)
				continue;

			std::cout << v->to_string() << "\n";
			if (rule.map_index(v->get_match_index()) == token_id::prep_def){
				symbols.add(v->get_value(), std::move(v->query<preprocessor_token::define>()->get_list()));
			}
		}

		auto ender = 0;
		ender += 0;
	}

	{
		rule rule;
		file ss("test/sample.txt");
		defined_symbols symbols;
		source_guard guard;

		source_info info{
			symbols,
			ss,
			rule,
			guard,
			&token_id_compare_collection::skip,
			&formatter::linked_collection::last
		};

		file::token_type v;
		while (ss.has_more()){
			if ((v = ss.next(info)) != nullptr && rule.map_index(v->get_match_index()) == token_id::prep_def){
				symbols.add(v->get_value(), std::move(v->query<preprocessor_token::define>()->get_list()));
			}
		}

		auto ender = 0;
		ender += 0;
	}

	return 0;
}