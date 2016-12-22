#include <iostream>

#include "lexer/string_source.h"
#include "lexer/token_formatter.h"
#include "lexer/file_source.h"
#include "lexer/scanner.h"

#include "object/primitive_object.h"
#include "node/literal_node.h"
#include "common/env.h"

using namespace cscript;
using namespace cscript::common;
using namespace cscript::lexer;
using namespace cscript::lexer::source;

int main(){
	/*node::literal ln(72ll, "", type::id::llong);
	auto ob = ln.evaluate();*/

	object::primitive::numeric num(env::int_type);
	object::primitive::numeric num2(env::llong_type, 45ll);

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