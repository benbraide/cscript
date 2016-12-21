#include "literal_parser.h"

cscript::parser::generic::node_type cscript::parser::collection::literal::parse(parser_info &info){
	switch (info.source_info.rule.map_index(info.token->get_match_index())){
	case lexer::token_id::dec:
		return parse_dec_(info);
	case lexer::token_id::hex:
		return parse_hex_(info);
	case lexer::token_id::oct:
		return parse_oct_(info);
	case lexer::token_id::rad:
		return parse_rad_(info);
	case lexer::token_id::bin:
		return parse_bin_(info);
	case lexer::token_id::real:
		return parse_real_(info);
	case lexer::token_id::exp:
		return parse_exp_(info);
	case lexer::token_id::quote_sng:
		return parse_single_(info, false);
	case lexer::token_id::quote_dbl:
		return parse_double_(info, false);
	case lexer::token_id::quote_back:
		return parse_back_(info, false);
	case lexer::token_id::esc_quote_sng:
		return parse_single_(info, true);
	case lexer::token_id::esc_quote_dbl:
		return parse_double_(info, true);
	case lexer::token_id::esc_quote_back:
		return parse_back_(info, true);
	default:
		break;
	}

	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_dec_(parser_info &info){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_hex_(parser_info &info){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_oct_(parser_info &info){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_rad_(parser_info &info){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_bin_(parser_info &info){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_real_(parser_info &info){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_exp_(parser_info &info){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_single_(parser_info &info, bool escaped){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_double_(parser_info &info, bool escaped){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_back_(parser_info &info, bool escaped){
	return nullptr;
}
