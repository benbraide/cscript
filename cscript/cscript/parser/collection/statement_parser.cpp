#include "statement_parser.h"
#include "../../common/env.h"

cscript::parser::generic::node_type cscript::parser::collection::statement::parse(){
	if (common::env::error.has())
		return nullptr;

	{//Get next
		lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
		if ((common::env::parser_info.token = common::env::source_info.source->peek(common::env::source_info)) == nullptr)
			return nullptr;
	}

	auto token = common::env::parser_info.token;
	switch (common::env::source_info.rule->map_index(token->get_match_index())){
	case lexer::token_id::prep_incl:
	{
		common::env::source_info.source->ignore_one(token);
		auto include_token = token->query<lexer::preprocessor_token::include>();
		common::env::include(include_token->get_value(), include_token->is_absolute());
		return parse();
	}
	case lexer::token_id::prep_def:
		common::env::source_info.source->ignore_one(token);
		common::env::source_info.symbols->add(token->get_value(), std::move(token->query<lexer::preprocessor_token::define>()->get_list()));
		return parse();
	case lexer::token_id::prep_undef:
		common::env::source_info.source->ignore_one(token);
		common::env::source_info.symbols->remove(token->get_value());
		return parse();
	case lexer::token_id::prep_ifdef:
	case lexer::token_id::prep_ifndef:
		common::env::source_info.source->ignore_one(token);
		common::env::source_guard.begin(token->query<lexer::preprocessor_token::conditional>()->is_rejected());
		if (common::env::source_guard.is_rejected())
			ignore_inactive_block_();

		return parse();
	case lexer::token_id::prep_else:
		common::env::source_info.source->ignore_one(token);
		if (common::env::source_guard.is_empty())
			return common::env::error.set("Invalid '#else' encountered", token->get_index());

		if (!common::env::source_guard.is_rejected())
			ignore_inactive_block_();

		return parse();
	case lexer::token_id::prep_endif:
		common::env::source_info.source->ignore_one(token);
		if (common::env::source_guard.is_empty())
			return common::env::error.set("Invalid '#endif' encountered", token->get_index());

		common::env::source_guard.end();
		return parse();
	case lexer::token_id::prep_pragma:
		common::env::source_info.source->ignore_one(token);
		return parse();
	default:
		break;
	}

	auto value = common::env::expression_parser.parse();
	if (common::env::error.has())
		return nullptr;

	if (value != nullptr && !value->is(node::id::block))
		return check_for_semicolon_(token->get_index(), value, false);

	return (value == nullptr) ? check_for_semicolon_(token->get_index(), value, true) : value;
}

void cscript::parser::collection::statement::ignore_inactive_block_(){
	lexer::token_id id;
	lexer::generic_source::token_type token;
	while (common::env::source_info.source->has_more()){
		if ((token = common::env::source_info.source->peek(common::env::source_info)) == nullptr)
			break;

		id = common::env::source_info.rule->map_index(token->get_match_index());
		if (id == lexer::token_id::prep_else || id == lexer::token_id::prep_endif)
			break;

		common::env::source_info.source->ignore(common::env::source_info);
	}
}

cscript::parser::generic::node_type cscript::parser::collection::statement::check_for_semicolon_(
	const lexer::token::index &index, node_type value, bool optional){
	auto halt = common::env::source_info.halt;
	lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);

	common::env::source_info.halt = { lexer::token_id::nil };//Disable halt
	auto token = common::env::source_info.source->peek(common::env::source_info);
	common::env::source_info.halt = halt;//Restore halt

	if (token == nullptr || common::env::source_info.rule->map_index(token->get_match_index()) != lexer::token_id::semi_colon)
		return optional ? value : common::env::error.set("Statement not terminated", index);

	common::env::source_info.source->ignore(common::env::source_info);
	return value;
}
