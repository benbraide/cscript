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

int main(){
	rule rule;
	file ss("test/sample.txt");
	defined_symbols symbols;
	source_guard guard;

	env::source_info = std::make_shared<source_info>(
		symbols,
		ss,
		rule,
		guard,
		&token_id_compare_collection::skip,
		&formatter::linked_collection::last
	);

	while (ss.has_more()){
		auto n = env::expression_parser.parse();
		if (n != nullptr){
			auto o = n->evaluate();
			auto i = o->query<object::primitive::numeric>()->get_value<int>();
			i += 0;
		}
		/*if (rule.map_index(v->get_match_index()) == token_id::prep_def){
			symbols.add(v->get_value(), std::move(v->query<preprocessor_token::define>()->get_list()));
		}*/
	}

	return 0;
}