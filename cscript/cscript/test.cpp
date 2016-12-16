#include <iostream>

#include "lexer/string_source.h"
#include "lexer/token_formatter.h"
#include "lexer/file_source.h"

using namespace cscript::lexer;
using namespace cscript::lexer::source;

int main(){
	rule rule;
	file ss("test/sample.txt");
	formatter::string fs;

	source_info info{
		ss,
		rule,
		nullptr,
		&fs
	};

	while (ss.has_more()){
		auto v = ss.next(info);
		std::cout << v->get_value() << "\n";
	}

	return 0;
}