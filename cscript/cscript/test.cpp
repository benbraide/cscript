#include <iostream>

#include "lexer/string_source.h"
#include "lexer/token_formatter.h"
#include "lexer/file_source.h"

using namespace cscript::lexer;
using namespace cscript::lexer::source;

int main(){
	rule rule;
	file ss("test/sample.txt");
	defined_symbols symbols;

	source_info info{
		symbols,
		ss,
		rule,
		&token_id_compare_collection::skip,
		&formatter::linked_collection::last
	};

	while (ss.has_more()){
		auto v = ss.next(info);
		std::cout << v->to_string() << "\n";
	}

	return 0;
}