#include "term_parser.h"
#include "../../object/pointer_object.h"

cscript::parser::generic::node_type cscript::parser::collection::term::parse(){
	if (common::env::error.has())
		return nullptr;

	node_type value;
	switch (common::env::source_info->rule.map_index(common::env::parser_info.token->get_match_index())){
	case lexer::token_id::false_:
		value = std::make_shared<node::literal>(common::env::parser_info.token->get_index(), lexer::token_id::false_,
			common::env::parser_info.token->get_value(), [](const std::string &value){
			return std::make_shared<object::primitive::boolean>(common::env::temp_address_space, type::boolean_value_type::false_);
		});
		break;
	case lexer::token_id::true_:
		value = std::make_shared<node::literal>(common::env::parser_info.token->get_index(), lexer::token_id::true_,
			common::env::parser_info.token->get_value(), [](const std::string &value){
			return std::make_shared<object::primitive::boolean>(common::env::temp_address_space, type::boolean_value_type::true_);
		});
		break;
	case lexer::token_id::nan:
		value = std::make_shared<node::literal>(common::env::parser_info.token->get_index(), lexer::token_id::true_,
			common::env::parser_info.token->get_value(), [](const std::string &value){
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space,
				common::env::ullong_type, std::stoull(value));
		});
		break;
	case lexer::token_id::nullptr_:
		value = std::make_shared<node::literal>(common::env::parser_info.token->get_index(), lexer::token_id::true_,
			common::env::parser_info.token->get_value(), [](const std::string &value){
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

	return nullptr;
}
