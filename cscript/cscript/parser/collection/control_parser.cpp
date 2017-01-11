#include "control_parser.h"
#include "../../common/env.h"

cscript::parser::generic::node_type cscript::parser::collection::control::parse(){
	if (common::env::parser_info.token == nullptr || common::env::error.has())
		return nullptr;

	switch (common::env::source_info.rule->map_index(common::env::parser_info.token->get_match_index())){
	case lexer::token_id::if_:
		return parse_control_<common::env, node::if_selection<common::env> >();
	case lexer::token_id::unless:
		return parse_control_<common::env, node::unless_selection<common::env> >();
	case lexer::token_id::while_:
		return parse_control_<common::env, node::while_iteration<common::env> >();
	case lexer::token_id::until:
		return parse_control_<common::env, node::until_iteration<common::env> >();
	case lexer::token_id::do_:
		return parse_do_while_();
	case lexer::token_id::for_:
		return parse_for_();
	case lexer::token_id::try_:
		return parse_try_or_finally_(false);
	case lexer::token_id::catch_:
		return common::env::error.set("'catch' found without matching target", common::env::parser_info.token->get_index());
	case lexer::token_id::finally_:
		return common::env::error.set("'finally' found without matching target", common::env::parser_info.token->get_index());
	case lexer::token_id::else_:
		return common::env::error.set("'else' found without matching target", common::env::parser_info.token->get_index());
	default:
		break;
	}

	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::control::parse_do_while_(){
	//do{ <body> } while(<predicate>)
	auto index = common::env::parser_info.token->get_index();

	lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
	common::env::source_info.source->ignore(common::env::source_info);

	auto body = parse_body_(index, true);
	if (common::env::error.has())
		return nullptr;

	auto next = common::env::source_info.source->next(common::env::source_info);
	if (next == nullptr || common::env::source_info.rule->map_index(next->get_match_index()) != lexer::token_id::while_)
		return common::env::error.set("Expected 'while'", index);

	auto predicate = parse_predicate_(index);
	if (common::env::error.has())
		return nullptr;

	next = common::env::source_info.source->next(common::env::source_info);
	if (next == nullptr || common::env::source_info.rule->map_index(next->get_match_index()) != lexer::token_id::semi_colon)
		return common::env::error.set("Expected a ';' after ')'", index);

	return std::make_shared<node::do_while_iteration<common::env> >(index, predicate, body);
}

cscript::parser::generic::node_type cscript::parser::collection::control::parse_for_(){
	//for (<initialization>; <predicate>; <update>)[{] <body> [}]
	auto index = common::env::parser_info.token->get_index();

	lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
	common::env::source_info.source->ignore(common::env::source_info);

	auto next = common::env::source_info.source->peek(common::env::source_info);
	if (next == nullptr)
		return common::env::error.set("Expected a '(' or 'each'", index);

	auto id = common::env::source_info.rule->map_index(next->get_match_index());
	if (id == lexer::token_id::identifier && next->get_value() == "each")
		return parse_for_each_(index);

	if (id != lexer::token_id::open_par)
		return common::env::error.set("Expected a '('", index);

	common::env::source_info.source->ignore(common::env::source_info);
	auto init = common::env::statement_parser.parse();
	if (common::env::error.has())
		return nullptr;

	auto predicate = common::env::builder.parse_single(builder::halt_info{ lexer::token_id::semi_colon });
	if (common::env::error.has())
		return nullptr;

	auto update = common::env::builder.parse_single(builder::halt_info{ lexer::token_id::close_par });
	if (common::env::error.has())
		return nullptr;

	auto body = parse_body_(index);
	if (common::env::error.has())
		return nullptr;

	auto else_ = parse_else_();
	if (common::env::error.has())
		return nullptr;

	return std::make_shared<node::for_iteration>(index, init, predicate, update, body, else_);
}

cscript::parser::generic::node_type cscript::parser::collection::control::parse_for_each_(const lexer::token::index &index){
	//for each (<declaration> : <expression>)[{] <body> [}]
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::control::parse_try_or_finally_(bool is_finally){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::control::parse_catch_or_finally_(){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::control::parse_else_(){
	auto next = common::env::source_info.source->peek(common::env::source_info);
	if (next == nullptr || common::env::source_info.rule->map_index(next->get_match_index()) != lexer::token_id::else_)
		return nullptr;

	common::env::source_info.source->ignore(common::env::source_info);
	return parse_body_(next->get_index());
}

cscript::parser::generic::node_type cscript::parser::collection::control::parse_predicate_(const lexer::token::index &index){
	auto next = common::env::source_info.source->next(common::env::source_info);
	if (next == nullptr || common::env::source_info.rule->map_index(next->get_match_index()) != lexer::token_id::open_par)
		return common::env::error.set("Expected a '('", index);

	auto value = common::env::builder.parse_single(builder::halt_info{ lexer::token_id::close_par });
	if (common::env::error.has())
		return nullptr;

	if (value == nullptr)
		return common::env::error.set("Bad expression", index);

	return value;
}

cscript::parser::generic::node_type cscript::parser::collection::control::parse_body_(const lexer::token::index &index, bool block){
	auto next = common::env::source_info.source->peek(common::env::source_info);
	if (next == nullptr || common::env::source_info.rule->map_index(next->get_match_index()) != lexer::token_id::open_cur){
		if (block)
			return common::env::error.set("Expected a '{'", index);
		return common::env::statement_parser.parse();
	}

	common::env::source_info.source->ignore(common::env::source_info);
	auto value = common::env::builder.parse_block();
	if (common::env::error.has())
		return nullptr;

	if (value == nullptr)
		return common::env::error.set("Bad control body", index);

	return value;
}
