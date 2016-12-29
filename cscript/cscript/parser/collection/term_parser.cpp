#include "term_parser.h"
#include "../../object/pointer_object.h"

cscript::parser::generic::node_type cscript::parser::collection::term::parse(){
	if (common::env::error.has())
		return nullptr;

	node_type value;
	auto token = common::env::parser_info.token;

	switch (common::env::source_info->rule.map_index(token->get_match_index())){
	case lexer::token_id::false_:
		value = std::make_shared<node::literal>(token->get_index(), lexer::token_id::false_,
			token->get_value(), [](const std::string &value){
			return std::make_shared<object::primitive::boolean>(common::env::temp_address_space, type::boolean_value_type::false_);
		});
		break;
	case lexer::token_id::true_:
		value = std::make_shared<node::literal>(token->get_index(), lexer::token_id::true_,
			token->get_value(), [](const std::string &value){
			return std::make_shared<object::primitive::boolean>(common::env::temp_address_space, type::boolean_value_type::true_);
		});
		break;
	case lexer::token_id::nan:
		value = std::make_shared<node::literal>(token->get_index(), lexer::token_id::true_,
			token->get_value(), [](const std::string &value){
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space,
				common::env::ullong_type, std::stoull(value));
		});
		break;
	case lexer::token_id::nullptr_:
		value = std::make_shared<node::literal>(token->get_index(), lexer::token_id::true_,
			token->get_value(), [](const std::string &value){
			return std::make_shared<object::pointer>(common::env::temp_address_space,
				memory::virtual_address::value_info{ &common::env::address_space, 0, 0 });
		});
		break;
	default:
		break;
	}

	if (value != nullptr || (value = common::env::literal_parser.parse()) != nullptr){//Literal value parsed
		lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
		common::env::source_info->source.ignore(*common::env::source_info);
		return value;
	}

	if ((value = common::env::unary_operator_parser.parse()) != nullptr)//Unary operator parsed
		return value;

	if (common::env::parser_info.context->query<context::expression>() == nullptr){
		switch (common::env::source_info->rule.map_index(token->get_match_index())){
		case lexer::token_id::open_par:
		{
			auto operand = common::env::builder.parse_list(builder::halt_info{ lexer::token_id::close_par });
			if (common::env::error.has())
				return nullptr;

			if (operand->query<node::collection>()->get_list().empty())
				return common::env::error.set("", token->get_index());

			return common::env::parser_info.left_operand = std::make_shared<node::unary_operator>(token->get_index(),
				node::unary_operator::info_type{ true, lexer::operator_id::call, token->get_value() }, operand);
		}
		case lexer::token_id::open_sq:
		{
			auto operand = common::env::builder.parse_list(builder::halt_info{ lexer::token_id::close_sq });
			if (common::env::error.has())
				return nullptr;

			return common::env::parser_info.left_operand = std::make_shared<node::unary_operator>(token->get_index(),
				node::unary_operator::info_type{ true, lexer::operator_id::index, token->get_value() }, operand);
		}
		default:
			break;
		}
	}

	return nullptr;
}
