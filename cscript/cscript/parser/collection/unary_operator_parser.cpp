#include "unary_operator_parser.h"
#include "../../common/env.h"

cscript::parser::generic::node_type cscript::parser::collection::unary_operator::parse(){
	if (common::env::parser_info.token == nullptr || common::env::error.has())
		return nullptr;

	auto token = common::env::parser_info.token;
	auto operator_token = token->query<lexer::operator_token>();
	if (operator_token == nullptr)
		return nullptr;

	auto precedence = get_precedence_(*operator_token);
	if (precedence < 0)
		return nullptr;

	{//Ignore token
		lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
		common::env::source_info->source.ignore(*common::env::source_info);
	}

	context::expression context(precedence, common::env::parser_info.context);
	common::env::parser_info.context = &context;

	auto operand = common::env::expression_parser.parse();
	common::env::parser_info.context = context.get_parent();//Restore context

	if (common::env::error.has())
		return nullptr;

	if (operand == nullptr){

	}

	return std::make_shared<node::unary_operator>(operator_token->get_index(),
		node::unary_operator::info_type{ true, operator_token->get_id(), operator_token->get_value() }, operand);
}

int cscript::parser::collection::unary_operator::get_precedence_(lexer::operator_token &token){
	switch (token.get_id()){
	case lexer::operator_id::throw_:
		return 1;
	case lexer::operator_id::sizeof_:
	case lexer::operator_id::typeof_:
	case lexer::operator_id::new_:
	case lexer::operator_id::delete_:
	case lexer::operator_id::bitwise_and:
	case lexer::operator_id::times:
	case lexer::operator_id::plus:
	case lexer::operator_id::minus:
	case lexer::operator_id::decrement:
	case lexer::operator_id::increment:
	case lexer::operator_id::relational_not:
	case lexer::operator_id::bitwise_inverse:
	case lexer::operator_id::unknown:
		return 13;
	default:
		break;
	}

	return -1;
}
