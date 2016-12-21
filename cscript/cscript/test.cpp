#include <iostream>

#include "lexer/string_source.h"
#include "lexer/token_formatter.h"
#include "lexer/file_source.h"
#include "lexer/scanner.h"

#include "object/primitive_object.h"

using namespace cscript;
using namespace cscript::lexer;
using namespace cscript::lexer::source;

int main(){
	memory::pool pool;
	memory::virtual_address va(pool);

	object::primitive::numeric num(va, static_cast<memory::virtual_address::size_type>(sizeof(int)));
	object::primitive::numeric num2(va, 45ll);

	auto ll = memory::pool::convert_unchecked<long long>(num2.get_memory().base);

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

	auto starter = 0;
	while (ss.has_more()){
		auto v = ss.next(info);
		if (v == nullptr)
			continue;

		std::cout << v->to_string() << "\n";
		if (rule.map_index(v->get_match_index()) == token_id::prep_def){
			symbols.add(v->get_value(), std::move(v->query<preprocessor_token::define>()->get_list()));
		}
	}

	return 0;
}