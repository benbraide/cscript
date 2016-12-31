#include "expression_parser.h"
#include "../../common/env.h"

cscript::parser::generic::node_type cscript::parser::collection::expression::parse(){
	if (common::env::error.has())
		return nullptr;

	{//Peek next
		lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
		common::env::parser_info.token = common::env::source_info->source.peek(*common::env::source_info);
	}

	save_left_operand save_left_operand(common::env::parser_info);
	if ((common::env::parser_info.left_operand = common::env::term_parser.parse()) == nullptr || common::env::error.has())
		return nullptr;

	{//Peek next
		lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
		common::env::parser_info.token = common::env::source_info->source.peek(*common::env::source_info);
	}

	auto expression = common::env::binary_operator_parser.parse();
	if (common::env::error.has())
		return nullptr;

	common::env::parser_info.token = nullptr;
	if (expression == nullptr)
		expression = common::env::parser_info.left_operand;
	else
		common::env::parser_info.left_operand = expression;

	if (!CSCRIPT_IS(common::env::parser_info.states, state::typename_) &&
		common::env::parser_info.context->query<context::expression>() == nullptr){
		expression = common::env::declaration_parser.parse();
		if (common::env::error.has())
			return nullptr;

		if (expression == nullptr)
			expression = common::env::parser_info.left_operand;
	}

	return expression;
}
