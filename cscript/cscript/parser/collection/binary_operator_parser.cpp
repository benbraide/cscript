#include "binary_operator_parser.h"
#include "../../common/env.h"

cscript::parser::generic::node_type cscript::parser::collection::binary_operator::parse(){
	if (common::env::parser_info.token == nullptr || common::env::error.has())
		return nullptr;

	auto precedence = -1, type = 0;
	auto id = lexer::operator_id::nil;

	auto token = common::env::parser_info.token;
	auto operator_token = token->query<lexer::operator_token>();

	if (operator_token == nullptr){
		switch (common::env::source_info.rule->map_index(token->get_match_index())){
		case lexer::token_id::comma:
			if (CSCRIPT_IS(common::env::parser_info.states, state::list))
				return nullptr;

			precedence = 0;
			id = lexer::operator_id::comma;

			break;
		case lexer::token_id::open_par:
			type = 1;
			precedence = 14;
			id = lexer::operator_id::call;
			break;
		case lexer::token_id::open_sq:
			type = 2;
			precedence = 14;
			id = lexer::operator_id::index;
			break;
		default:
			return nullptr;
		}
	}

	if (precedence < 0){
		lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
		auto next = common::env::source_info.source->peek_after(token, common::env::source_info);
		if (next == nullptr || common::env::source_info.rule->map_index(next->get_match_index()) == lexer::token_id::semi_colon){
			precedence = get_unary_precedence_(*operator_token);
			type = 3;
		}
		else//Binary
			precedence = get_precedence_(*operator_token);

		if (precedence < 0)
			return nullptr;

		id = operator_token->get_id();
	}

	auto previous_precedence = -1;
	auto expression_context = common::env::parser_info.context->query<context::expression>();
	if (expression_context != nullptr)
		previous_precedence = expression_context->get_precedence();

	if (precedence < previous_precedence || (precedence == previous_precedence && precedence != 1))
		return nullptr;

	{//Ignore token
		lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
		common::env::source_info.source->ignore(common::env::source_info);
	}

	auto left_operand = common::env::parser_info.left_operand;
	if (type == 0){//Binary
		context::expression context(precedence, common::env::parser_info.context);
		common::env::parser_info.context = &context;

		auto states = common::env::parser_info.states;
		CSCRIPT_SET(common::env::parser_info.states, state::unary);

		auto operand = common::env::expression_parser.parse();
		common::env::parser_info.context = context.get_parent();//Restore context
		common::env::parser_info.states = states;//Restore states

		if (common::env::error.has())
			return nullptr;

		if (operand == nullptr){//Unary right
			common::env::parser_info.left_operand = std::make_shared<node::unary_operator>(left_operand->get_index(),
				node::unary_operator::info_type{ false, id, token->get_value() }, left_operand);
		}
		else{
			common::env::parser_info.left_operand = std::make_shared<node::binary_operator>(left_operand->get_index(),
				node::binary_operator::info_type{ id, token->get_value() }, left_operand, operand);
		}
	}
	else if (type == 1){// ()
		auto operand = common::env::builder.parse_list(builder::halt_info{ lexer::token_id::close_par });
		if (common::env::error.has())
			return nullptr;

		common::env::parser_info.left_operand = std::make_shared<node::binary_operator>(left_operand->get_index(),
			node::binary_operator::info_type{ id, token->get_value() }, left_operand, operand);
	}
	else if (type == 2){// []
		auto operand = common::env::builder.parse_single(builder::halt_info{ lexer::token_id::close_sq });
		if (common::env::error.has())
			return nullptr;

		common::env::parser_info.left_operand = std::make_shared<node::binary_operator>(left_operand->get_index(),
			node::binary_operator::info_type{ id, token->get_value() }, left_operand, operand);
	}
	else{//Unary right
		common::env::parser_info.left_operand = std::make_shared<node::unary_operator>(left_operand->get_index(),
			node::unary_operator::info_type{ false, id, token->get_value() }, left_operand);
	}

	{//Peek next
		lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
		common::env::parser_info.token = common::env::source_info.source->peek(common::env::source_info);
	}

	return parse();
}

int cscript::parser::collection::binary_operator::get_precedence_(lexer::operator_token &token){
	switch (token.get_id()){
	case lexer::operator_id::comma:
		return CSCRIPT_IS(common::env::parser_info.states, state::list) ? -1 : 0;
	case lexer::operator_id::ternary:
	case lexer::operator_id::assignment:
	case lexer::operator_id::compound_bitwise_or:
	case lexer::operator_id::compound_bitwise_xor:
	case lexer::operator_id::compound_bitwise_and:
	case lexer::operator_id::compound_left_shift:
	case lexer::operator_id::compound_right_shift:
	case lexer::operator_id::compound_plus:
	case lexer::operator_id::compound_minus:
	case lexer::operator_id::compound_times:
	case lexer::operator_id::compound_divide:
	case lexer::operator_id::compound_modulus:
		return 1;
	case lexer::operator_id::relational_or:
		return 2;
	case lexer::operator_id::relational_and:
		return 3;
	case lexer::operator_id::bitwise_or:
		return CSCRIPT_IS(common::env::parser_info.states, state::typename_) ? 16 : 4;
	case lexer::operator_id::bitwise_xor:
		return 5;
	case lexer::operator_id::bitwise_and:
		return 6;
	case lexer::operator_id::equality:
	case lexer::operator_id::explicit_equality:
	case lexer::operator_id::inverse_equality:
	case lexer::operator_id::explicit_inverse_equality:
		return 7;
	case lexer::operator_id::less:
	case lexer::operator_id::less_or_equal:
	case lexer::operator_id::more_or_equal:
	case lexer::operator_id::more:
		return 8;
	case lexer::operator_id::left_shift:
	case lexer::operator_id::right_shift:
		return 9;
	case lexer::operator_id::unknown:
		return 10;
	case lexer::operator_id::plus:
	case lexer::operator_id::minus:
		return 11;
	case lexer::operator_id::times:
	case lexer::operator_id::divide:
	case lexer::operator_id::modulus:
		return 12;
	case lexer::operator_id::member_access:
	case lexer::operator_id::member_pointer_access:
		return 14;
	case lexer::operator_id::scope_resolution:
		return 18;
	default:
		break;
	}

	return -1;
}

int cscript::parser::collection::binary_operator::get_unary_precedence_(lexer::operator_token &token){
	switch (token.get_id()){
	case lexer::operator_id::decrement:
	case lexer::operator_id::increment:
	case lexer::operator_id::unknown:
		return 14;
	}

	return -1;
}
