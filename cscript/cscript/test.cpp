#include <iostream>

#include "lexer/string_source.h"
#include "lexer/token_formatter.h"
#include "lexer/file_source.h"
#include "lexer/scanner.h"

#include "object/numeric_object.h"
#include "node/literal_node.h"
#include "common/env.h"

using namespace cscript;
using namespace cscript::common;
using namespace cscript::lexer;
using namespace cscript::lexer::source;
using namespace cscript::parser;

int main(){
	env::initialize();

	rule rule;
	source::string ss("");
	defined_symbols symbols;

	cscript::common::env::source_info = source_info{
		&symbols,
		&ss,
		&rule,
		&token_id_compare_collection::skip,
		&formatter::linked_collection::last
	};

	std::string buffer;
	while (true){
		std::cout << "> ";
		std::getline(std::cin, buffer);
		if (buffer.empty())
			continue;

		ss.reset(buffer);
		env::parse(ss);
		env::error.report();
	}

	return 0;
}