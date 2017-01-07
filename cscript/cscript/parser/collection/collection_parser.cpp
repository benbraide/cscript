#include "collection_parser.h"
#include "../../common/env.h"

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse(const options &options){
	if (common::env::error.has())
		return nullptr;

	lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
	auto terminated = false, has_trailing_delimiter = true;
	auto token = common::env::source_info.source->peek(common::env::source_info);
	if (token == nullptr){
		if (options.terminator.id != lexer::token_id::nil)
			return common::env::error.set("", common::env::source_info.source->get_index());
		return std::make_shared<node::collection>(common::env::source_info.source->get_index());
	}

	node_type node;
	token_id_type token_id;
	auto collection = std::make_shared<node::collection>(token->get_index());

	lexer::source_info::halt_info halt{};
	auto same_halt = (common::env::source_info.halt == options.terminator);
	if (!same_halt)
		halt = common::env::source_info.halt;

	while (common::env::source_info.source->has_more()){
		if (same_halt)//Disable halt
			common::env::source_info.halt = {};
		else//Use previous halt
			common::env::source_info.halt = halt;

		token = common::env::source_info.source->peek(common::env::source_info);
		if (same_halt)//Restore halt
			common::env::source_info.halt = options.terminator;

		if (token == nullptr)
			break;

		token_id = common::env::source_info.rule->map_index(token->get_match_index());
		if (token_id == options.terminator.id && (options.terminator.value.empty() || token->get_value() == options.terminator.value)){
			common::env::source_info.source->ignore(common::env::source_info);
			terminated = true;
			break;
		}

		if (token_id == options.delimiter.id && (options.delimiter.value.empty() || token->get_value() == options.delimiter.value)){
			if (token_id != token_id_type::semi_colon)
				return common::env::error.set("", collection->get_index());

			common::env::source_info.source->ignore(common::env::source_info);
			has_trailing_delimiter = true;

			continue;
		}

		if (token_id == token_id_type::semi_colon)
			break;

		node = nullptr;
		if (!same_halt)
			common::env::source_info.halt = options.delimiter;//Enable halt

		if (options.before_callback != nullptr && (!options.before_callback(node) || common::env::error.has()))
			break;

		if (node == nullptr && ((node = options.parser.parse()) == nullptr || common::env::error.has()))
			break;
		
		if (options.after_callback != nullptr && (!options.after_callback(node) || common::env::error.has()))
			break;

		collection->append(node);
		if (same_halt)//Disable halt
			common::env::source_info.halt = {};
		else//Use previous halt
			common::env::source_info.halt = halt;

		token = common::env::source_info.source->peek(common::env::source_info);
		if (same_halt)//Restore halt
			common::env::source_info.halt = options.terminator;

		if (token == nullptr)
			break;

		token_id = common::env::source_info.rule->map_index(token->get_match_index());
		if (token_id == options.terminator.id && (options.terminator.value.empty() || token->get_value() == options.terminator.value)){
			common::env::source_info.source->ignore(common::env::source_info);
			terminated = true;
			break;
		}
		else if (token_id == options.delimiter.id && (options.delimiter.value.empty() || token->get_value() == options.delimiter.value)){
			common::env::source_info.source->ignore(common::env::source_info);
			has_trailing_delimiter = true;
		}
		else if (node->is(options.alternate_delimiter))
			has_trailing_delimiter = true;
		else if (options.delimiter.id != token_id_type::semi_colon || !node->is(node::id::block))//Error
			return common::env::error.set("", collection->get_index());
	}

	if (!same_halt)//Restore halt info
		common::env::source_info.halt = halt;

	if (common::env::error.has())
		return nullptr;

	if ((!terminated && options.terminator.id != lexer::token_id::nil) || (has_trailing_delimiter && options.no_trailing_delimiter) ||
		(!has_trailing_delimiter && options.require_trailing_delimiter)){
		return common::env::error.set("", collection->get_index());
	}

	return collection;
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_list(
	const halt_info &terminator, const halt_info &delimiter, bool no_trailing_delimiter){
	return parse(options{
		common::env::expression_parser,
		terminator,
		delimiter,
		no_trailing_delimiter
	});
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_list(
	const halt_info &terminator, bool no_trailing_delimiter){
	return parse_list(terminator, halt_info{ token_id_type::comma }, no_trailing_delimiter);
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_list(const halt_info &terminator){
	return parse_list(terminator, halt_info{ token_id_type::comma }, true);
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_block(const halt_info &terminator,
	callback_type before_callback, callback_type after_callback){
	return parse(options{
		common::env::statement_parser,
		terminator,
		halt_info{ lexer::token_id::semi_colon },
		false,
		before_callback,
		after_callback,
		true,
		node::id::block
	});
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_block(const halt_info &terminator){
	return parse_block(terminator, nullptr, nullptr);
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_single(const halt_info &terminator){
	lexer::source_info::halt_info halt{};
	auto same_halt = (common::env::source_info.halt == terminator);
	if (!same_halt)
		halt = common::env::source_info.halt;
	else//Disable halt
		common::env::source_info.halt = {};

	lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
	auto token = common::env::source_info.source->peek(common::env::source_info);

	if (token == nullptr){
		if (same_halt)//Restore halt
			common::env::source_info.halt = terminator;

		if (terminator.id != lexer::token_id::nil)
			return common::env::error.set("", common::env::source_info.source->get_index());
		return nullptr;
	}

	auto token_id = common::env::source_info.rule->map_index(token->get_match_index());
	if (token_id == terminator.id && (terminator.value.empty() || token->get_value() == terminator.value)){
		common::env::source_info.source->ignore(common::env::source_info);
		if (same_halt)//Restore halt
			common::env::source_info.halt = terminator;

		return nullptr;
	}

	common::env::source_info.halt = terminator;//Enable halt
	auto node = common::env::expression_parser.parse();

	if (!same_halt)
		common::env::source_info.halt = halt;//Restore halt info

	if (common::env::error.has())
		return nullptr;

	if (terminator.id != lexer::token_id::nil){//Ensure next token is terminator
		if (same_halt)//Disable halt
			common::env::source_info.halt = {};

		token = common::env::source_info.source->peek(common::env::source_info);
		if (token == nullptr)
			return common::env::error.set("", common::env::source_info.source->get_index());

		token_id = common::env::source_info.rule->map_index(token->get_match_index());
		if (token_id != terminator.id || (!terminator.value.empty() && token->get_value() != terminator.value))
			return common::env::error.set("", common::env::source_info.source->get_index());

		common::env::source_info.source->ignore(common::env::source_info);
		if (same_halt)//Restore halt
			common::env::source_info.halt = terminator;
	}

	return node;
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_type(){
	auto states = common::env::parser_info.states;
	CSCRIPT_SET(common::env::parser_info.states, state::typename_);

	context::expression context(15, common::env::parser_info.context);
	common::env::parser_info.context = &context;

	auto value = common::env::expression_parser.parse();
	common::env::parser_info.context = context.get_parent();//Restore context
	common::env::parser_info.states = states;//Restore states

	if (common::env::error.has())
		return nullptr;

	return value;
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_expression(){
	auto states = common::env::parser_info.states;
	CSCRIPT_SET(common::env::parser_info.states, state::list);

	auto value = common::env::expression_parser.parse();
	common::env::parser_info.states = states;//Restore states

	if (common::env::error.has())
		return nullptr;

	return value;
}
