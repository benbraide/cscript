#include "collection_parser.h"

#include "../../common/env.h"
#include "../../node/declaration_node.h"

cscript::parser::collection::builder::node_type cscript::parser::collection::builder::parse(const options &options){
	if (common::env::error.has())
		return nullptr;

	lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
	auto halt = common::env::source_info.halt;
	common::env::source_info.halt = { lexer::token_id::nil };//Disable halt

	auto terminated = false, has_trailing_delimiter = false, first_entry = true;
	auto token = common::env::source_info.source->peek(common::env::source_info);

	if (token == nullptr){
		if (options.terminator.id != lexer::token_id::nil)
			return common::env::error.set("Statement not terminated", common::env::source_info.source->get_index());
		return std::make_shared<node::collection>(common::env::source_info.source->get_index());
	}

	node_type node;
	auto collection = std::make_shared<node::collection>(token->get_index());

	while (common::env::source_info.source->has_more()){
		auto match_type = get_match_type(options.terminator, options.delimiter);
		if (match_type == 2){//Delimiter
			if (first_entry || has_trailing_delimiter){
				common::env::error.set("Malformed list statement", collection->get_index());
				break;
			}

			has_trailing_delimiter = true;
		}
		else if (match_type != 0){
			if (match_type == 1)//Terminated
				terminated = true;
			break;
		}
		else if (options.delimiter.id != lexer::token_id::nil && node != nullptr &&
			!node->is(options.alternate_delimiter)){//Expects delimiter
			common::env::error.set("Delimiter expected", collection->get_index());
			break;
		}

		node = nullptr;
		common::env::source_info.halt = options.delimiter;//Enable delimiter halt
		if (options.before_callback != nullptr && (!options.before_callback(node) || common::env::error.has()))
			break;

		if (node == nullptr){
			node = options.parser.parse();
			if (common::env::error.has())
				break;

			if (options.after_callback != nullptr && (!options.after_callback(node) || common::env::error.has()))
				break;
		}

		if (node != nullptr){
			first_entry = false;
			collection->append(node);
			if (options.delimiter.id != lexer::token_id::nil)
				has_trailing_delimiter = false;
		}
	}

	common::env::source_info.halt = halt;//Restore halt
	if (common::env::error.has())
		return nullptr;

	if (!terminated && options.terminator.id != lexer::token_id::nil)
		return common::env::error.set("Statement not terminated", collection->get_index());

	if (has_trailing_delimiter && options.no_trailing_delimiter)
		return common::env::error.set("Trailing delimiter encountered", collection->get_index());

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
				common::env::error.set("Expected declaration as parameter");
				return true;
			}

			if (state <= 1){
				auto declaration_node = value->query<node::declaration>();
				if (declaration_node != nullptr && declaration_node->get_type()->is(node::id::variadic_type))
					state = 2;
				else if (value->query<node::initialization_declaration>() != nullptr)
					state = 1;
				else if (state > 0)
					common::env::error.set("Malformed parameter list");
			}
			else
				common::env::error.set("Malformed parameter list");

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
		halt_info{ lexer::token_id::nil },
		false,
		before_callback,
		after_callback,
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

	auto match_type = get_match_type(terminator, halt_info{ lexer::token_id::nil });
	common::env::source_info.halt = halt;//Restore halt

	if (match_type != 1)//Not terminated
		return common::env::error.set("Statement not terminated", common::env::source_info.source->get_index());

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

int cscript::parser::collection::builder::get_match_type(const halt_info &terminator, const halt_info &delimiter){
	common::env::source_info.halt = { lexer::token_id::nil };//Disable halt
	auto token = common::env::source_info.source->peek(common::env::source_info);
	if (token == nullptr)
		return -1;

	auto token_id = common::env::source_info.rule->map_index(token->get_match_index());
	if (terminator.id != lexer::token_id::nil && token_id == terminator.id &&
		(terminator.value.empty() || token->get_value() == terminator.value)){
		common::env::source_info.source->ignore(common::env::source_info);
		return 1;
	}

	if (delimiter.id != lexer::token_id::nil && token_id == delimiter.id &&
		(delimiter.value.empty() || token->get_value() == delimiter.value)){
		common::env::source_info.source->ignore(common::env::source_info);
		return 2;
	}

	return 0;
}
