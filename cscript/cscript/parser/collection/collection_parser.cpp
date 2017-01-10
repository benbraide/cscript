#include "collection_parser.h"

#include "../../common/env.h"
#include "../../node/declaration_node.h"

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse(const options &options){
	if (common::env::error.has())
		return nullptr;

	lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
	auto halt = common::env::source_info.halt;
	common::env::source_info.halt = { lexer::token_id::nil };//Disable halt

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

	while (common::env::source_info.source->has_more()){
		common::env::source_info.halt = { lexer::token_id::nil };//Disable halt
		if ((token = common::env::source_info.source->peek(common::env::source_info)) == nullptr){
			if (node == nullptr)
				has_trailing_delimiter = false;
			break;
		}

		token_id = common::env::source_info.rule->map_index(token->get_match_index());
		if (token_id == options.terminator.id && (options.terminator.value.empty() || token->get_value() == options.terminator.value)){
			common::env::source_info.source->ignore(common::env::source_info);
			terminated = true;
			if (node == nullptr)
				has_trailing_delimiter = false;
			break;
		}

		if (token_id == options.delimiter.id && (options.delimiter.value.empty() || token->get_value() == options.delimiter.value)){
			if (has_trailing_delimiter && !options.require_trailing_delimiter){
				common::env::error.set("", collection->get_index());
				break;
			}

			common::env::source_info.source->ignore(common::env::source_info);
			has_trailing_delimiter = true;
		}
		else if (node != nullptr){//Expects delimiter
			if (!node->is(options.alternate_delimiter)){
				common::env::error.set("", collection->get_index());
				break;
			}

			has_trailing_delimiter = true;
		}
		else if (token_id == token_id_type::semi_colon)
			break;

		node = nullptr;
		common::env::source_info.halt = options.delimiter;//Enable delimiter halt

		if (options.before_callback != nullptr && (!options.before_callback(node) || common::env::error.has()))
			break;

		if (node == nullptr){
			node = options.parser.parse();
			if (common::env::error.has())
				return nullptr;

			if (node == nullptr){
				if (!options.require_trailing_delimiter)
					break;

				common::env::source_info.halt = { lexer::token_id::nil };//Disable halt
				if ((token = common::env::source_info.source->peek(common::env::source_info)) == nullptr)
					break;

				token_id = common::env::source_info.rule->map_index(token->get_match_index());
				if (token_id == options.terminator.id && (options.terminator.value.empty() || token->get_value() == options.terminator.value)){
					common::env::source_info.source->ignore(common::env::source_info);
					terminated = true;
					break;
				}

				if (token_id != options.delimiter.id || (!options.delimiter.value.empty() && token->get_value() != options.delimiter.value))
					break;

				continue;
			}
		}
		
		if (options.after_callback != nullptr && (!options.after_callback(node) || common::env::error.has()))
			break;

		collection->append(node);
		has_trailing_delimiter = false;
	}

	common::env::source_info.halt = halt;//Restore halt
	if (common::env::error.has())
		return nullptr;

	if ((!terminated && options.terminator.id != lexer::token_id::nil) || (has_trailing_delimiter && options.no_trailing_delimiter) ||
		(!has_trailing_delimiter && options.require_trailing_delimiter)){
		return common::env::error.set("", collection->get_index());
	}

	return collection;
}

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_parameters(const halt_info &terminator){
	auto state = 0;
	return parse(options{
		common::env::expression_parser,
		terminator,
		halt_info{ token_id_type::comma },
		true,
		[](node_type &value){
			value = common::env::declaration_parser.parse_parameter();
			return true;
		},
		[&state](node_type &value){
			if (!value->is(node::id::declaration)){
				common::env::error.set("");
				return true;
			}

			if (state <= 1){
				auto declaration_node = value->query<node::declaration>();
				if (declaration_node != nullptr && declaration_node->get_type()->is(node::id::variadic_type))
					state = 2;
				else if (value->query<node::initialization_declaration>() != nullptr)
					state = 1;
				else if (state > 0)
					common::env::error.set("");
			}
			else
				common::env::error.set("");

			return true;
		}
	});
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
	auto halt = common::env::source_info.halt;
	common::env::source_info.halt = terminator;//Enable halt

	auto node = common::env::expression_parser.parse();
	if (common::env::error.has()){
		common::env::source_info.halt = halt;//Restore halt
		return nullptr;
	}

	if (terminator.id != lexer::token_id::nil){//Ensure next token is terminator
		common::env::source_info.halt = { lexer::token_id::nil };//Disable halt
		auto token = common::env::source_info.source->peek(common::env::source_info);

		if (token == nullptr){
			common::env::source_info.halt = halt;//Restore halt
			return common::env::error.set("", common::env::source_info.source->get_index());
		}

		auto token_id = common::env::source_info.rule->map_index(token->get_match_index());
		if (token_id != terminator.id || (!terminator.value.empty() && token->get_value() != terminator.value)){
			common::env::source_info.halt = halt;//Restore halt
			return common::env::error.set("", common::env::source_info.source->get_index());
		}

		common::env::source_info.source->ignore(common::env::source_info);
		common::env::source_info.halt = halt;//Restore halt
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

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse_name(){
	context::expression context(17, common::env::parser_info.context);
	common::env::parser_info.context = &context;

	auto value = common::env::expression_parser.parse();
	common::env::parser_info.context = context.get_parent();//Restore context
	if (common::env::error.has())
		return nullptr;

	return value;
}
