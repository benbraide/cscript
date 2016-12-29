#include "collection_parser.h"
#include "../../common/env.h"

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse(const options &options){
	auto terminated = false, has_trailing_delimiter = true;
	auto collection = std::make_shared<node::collection>(common::env::parser_info.token->get_index());

	node_type node;
	token_type token;
	token_id_type token_id;

	auto halt = common::env::source_info->halt;
	while (common::env::source_info->source.has_more()){
		common::env::source_info->halt = {};
		if ((token = common::env::source_info->source.peek(*common::env::source_info)) == nullptr)
			break;

		token_id = common::env::source_info->rule.map_index(token->get_match_index());
		if (token_id == options.terminator.id && (options.terminator.value.empty() || token->get_value() == options.terminator.value)){
			common::env::source_info->source.ignore(*common::env::source_info);
			terminated = true;
			break;
		}

		if (token_id == options.delimiter.id && (options.delimiter.value.empty() || token->get_value() == options.delimiter.value)){
			if (token_id != token_id_type::semi_colon)
				return common::env::error.set("", collection->get_index());

			common::env::source_info->source.ignore(*common::env::source_info);
			has_trailing_delimiter = true;

			continue;
		}

		if (token_id == token_id_type::semi_colon)
			break;

		if (options.before_callback != nullptr && (!options.before_callback() || common::env::error.has()))
			break;

		common::env::source_info->halt = options.delimiter;
		if ((node = options.parser.parse()) == nullptr || common::env::error.has())
			break;

		if (options.after_callback != nullptr && (!options.after_callback(node) || common::env::error.has()))
			break;

		collection->append(node);
		if ((token = common::env::source_info->source.peek(*common::env::source_info)) == nullptr)
			break;

		token_id = common::env::source_info->rule.map_index(token->get_match_index());
		if (token_id == options.terminator.id && (options.terminator.value.empty() || token->get_value() == options.terminator.value)){
			common::env::source_info->source.ignore(*common::env::source_info);
			terminated = true;
			break;
		}
		else if (token_id == options.delimiter.id && (options.delimiter.value.empty() || token->get_value() == options.delimiter.value)){
			common::env::source_info->source.ignore(*common::env::source_info);
			has_trailing_delimiter = true;
		}
		else if (options.delimiter.id != token_id_type::semi_colon || !node->is(node::id::block))//Error
			return common::env::error.set("", collection->get_index());
	}

	common::env::source_info->halt = halt;//Restore halt info
	if (common::env::error.has())
		return nullptr;

	if ((!terminated && options.terminator.id != lexer::token_id::nil) || (has_trailing_delimiter && options.no_trailing_delimiter))
		return common::env::error.set("", collection->get_index());

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
	before_callback_type before_callback, after_callback_type after_callback){
	return parse(options{
		common::env::statement_parser,
		terminator,
		halt_info{ lexer::token_id::semi_colon },
		false,
		before_callback,
		after_callback
	});
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_block(const halt_info &terminator){
	return parse_block(terminator, nullptr, nullptr);
}
