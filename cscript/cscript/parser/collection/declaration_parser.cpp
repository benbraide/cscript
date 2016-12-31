#include "declaration_parser.h"
#include "collection_parser.h"
#include "../../common/env.h"

cscript::parser::generic::node_type cscript::parser::collection::declaration::parse(){
	auto operand = common::env::parser_info.left_operand;
	if (operand == nullptr || !(operand->is(node::id::identifier) || operand->is(node::id::type)))
		return nullptr;

	lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
	auto next = common::env::source_info->source.peek(*common::env::source_info);
	if (next == nullptr)
		return nullptr;

	node::generic::ptr_type id_node;
	auto id = common::env::source_info->rule.map_index(next->get_match_index());
	if (id != lexer::token_id::identifier){//Check operator | placeholder

	}
	else{//Identifier
		common::env::source_info->source.ignore(*common::env::source_info);
		id_node = std::make_shared<node::identifier>(next->get_index(), next->get_value());
	}

	if (id_node == nullptr)
		return nullptr;

	auto declaration = std::make_shared<node::declaration>(operand->get_index(), operand, id_node);
	return extend_(declaration);
}

cscript::parser::generic::node_type cscript::parser::collection::declaration::extend_(node_type value){
	auto next = common::env::source_info->source.peek(*common::env::source_info);
	if (next == nullptr)
		return value;

	auto id = common::env::source_info->rule.map_index(next->get_match_index());
	if (id == lexer::token_id::operator_symbol){//Check initialization
		auto operator_token = next->query<lexer::operator_token>();
		if (operator_token == nullptr || operator_token->get_id() != lexer::operator_id::assignment)
			return value;

		common::env::source_info->source.ignore(*common::env::source_info);
		auto right_value = common::env::builder.parse_expression();
		if (common::env::error.has())
			return nullptr;

		return extend_(std::make_shared<node::initialization_declaration>(value->get_index(), value, right_value));
	}
	
	if (id == lexer::token_id::comma){//Dependent declaration
		common::env::source_info->source.ignore(*common::env::source_info);
		auto next = common::env::source_info->source.peek(*common::env::source_info);
		if (next == nullptr)
			return value;

		node::generic::ptr_type id_node;
		auto id = common::env::source_info->rule.map_index(next->get_match_index());
		if (id != lexer::token_id::identifier){
			common::env::source_info->source.ignore(*common::env::source_info);
			id_node = std::make_shared<node::identifier>(next->get_index(), next->get_value());
		}

		if (id_node == nullptr)
			return nullptr;

		auto declaration = std::make_shared<node::dependent_declaration>(value->get_index(), value, id_node);
		return extend_(declaration);
	}
	
	if (id == lexer::token_id::open_par){//Function

	}
	
	if (id == lexer::token_id::open_sq){//Array

	}
	
	if (id == lexer::token_id::open_cur){//Uniform

	}

	return value;
}
