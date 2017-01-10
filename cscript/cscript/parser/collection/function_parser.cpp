#include "function_parser.h"

#include "../../common/env.h"

cscript::parser::generic::node_type cscript::parser::collection::function::parse(){
	if (common::env::error.has())
		return nullptr;

	auto operand = common::env::parser_info.left_operand;
	if (operand == nullptr)
		return nullptr;

	auto parameters = common::env::builder.parse_parameters();
	if (common::env::error.has())
		return nullptr;

	if (parameters == nullptr)
		return common::env::error.set("Missing function parameters", operand->get_index());

	node_type definition;
	auto next = common::env::source_info.source->peek(common::env::source_info);
	if (next != nullptr && common::env::source_info.rule->map_index(next->get_match_index()) == lexer::token_id::open_cur){
		common::env::source_info.source->ignore(common::env::source_info);

		auto states = common::env::parser_info.states;
		CSCRIPT_SET(common::env::parser_info.states, state::function);

		definition = common::env::builder.parse_block();
		common::env::parser_info.states = states;//Restore states

		if (common::env::error.has())
			return nullptr;

		if (definition == nullptr)
			return common::env::error.set("Missing function definition", operand->get_index());
	}

	return std::make_shared<node::function>(operand->get_index(), operand, parameters, definition);
}

cscript::parser::generic::node_type cscript::parser::collection::function::parse_constructor(){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::function::parse_destructor(){
	return nullptr;
}
