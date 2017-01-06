#include "term_parser.h"

#include "../../common/env.h"
#include "../../object/pointer_object.h"

cscript::parser::generic::node_type cscript::parser::collection::term::parse(){
	if (common::env::error.has())
		return nullptr;

	auto value = common::env::keyword_parser.parse();
	if (value != nullptr || common::env::error.has())
		return value;

	auto token = common::env::parser_info.token;
	auto id = common::env::source_info->rule.map_index(token->get_match_index());

	if (lexer::token_id_compare_collection::type.is(id)){
		lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
		common::env::source_info->source.ignore(*common::env::source_info);
		return std::make_shared<node::primitive_type>(token->get_index(), id);
	}

	if (is_storage_class_(id)){
		lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
		common::env::source_info->source.ignore(*common::env::source_info);

		auto index = token->get_index();
		auto attributes = convert_storage_class_(id);
		auto attribute = memory::virtual_address::attribute::nil;

		auto string_value = token->get_value();
		while (common::env::source_info->source.has_more()){
			if ((token = common::env::source_info->source.peek(*common::env::source_info)) == nullptr)
				break;

			id = common::env::source_info->rule.map_index(token->get_match_index());
			if (!is_storage_class_(id))
				break;

			string_value += (" " + token->get_value());
			attribute = convert_storage_class_(id);

			if (CSCRIPT_IS(attributes, attribute))
				return common::env::error.set("", index);

			CSCRIPT_SET(attributes, attribute);
			common::env::source_info->source.ignore(*common::env::source_info);
		}

		auto type = common::env::builder.parse_type();
		if (common::env::error.has())
			return nullptr;

		if (type == nullptr)
			return common::env::error.set("", index);

		return std::make_shared<node::type_with_storage_class>(index, attributes, type, string_value);
	}

	switch (id){
	case lexer::token_id::false_:
		value = std::make_shared<node::literal>(token->get_index(), lexer::token_id::false_,
			token->get_value(), [](const std::string &value){
			return std::make_shared<object::primitive::boolean>(type::boolean_value_type::false_);
		});
		break;
	case lexer::token_id::true_:
		value = std::make_shared<node::literal>(token->get_index(), lexer::token_id::true_,
			token->get_value(), [](const std::string &value){
			return std::make_shared<object::primitive::boolean>(type::boolean_value_type::true_);
		});
		break;
	case lexer::token_id::nan:
		value = std::make_shared<node::literal>(token->get_index(), lexer::token_id::true_,
			token->get_value(), [](const std::string &value){
			return std::make_shared<object::primitive::numeric>(common::env::ullong_type, std::stoull(value));
		});
		break;
	case lexer::token_id::nullptr_:
		value = std::make_shared<node::literal>(token->get_index(), lexer::token_id::true_,
			token->get_value(), [](const std::string &value){
			return std::make_shared<object::pointer>(0ull);
		});
		break;
	case lexer::token_id::identifier:
		value = std::make_shared<node::identifier>(token->get_index(), token->get_value());
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
		switch (id){
		case lexer::token_id::open_par:
		{
			auto operand = common::env::builder.parse_single(builder::halt_info{ lexer::token_id::close_par });
			if (common::env::error.has())
				return nullptr;

			if (operand = nullptr)
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

bool cscript::parser::collection::term::is_storage_class_(lexer::token_id id) const{
	switch (id){
	case lexer::token_id::static_:
	case lexer::token_id::const_:
	case lexer::token_id::final_:
	case lexer::token_id::ref:
	case lexer::token_id::private_:
	case lexer::token_id::protected_:
	case lexer::token_id::public_:
		return true;
	default:
		break;
	}

	return false;
}

cscript::memory::virtual_address::attribute cscript::parser::collection::term::convert_storage_class_(lexer::token_id id) const{
	switch (id){
	case lexer::token_id::static_:
		return memory::virtual_address::attribute::static_;
	case lexer::token_id::const_:
		return memory::virtual_address::attribute::constant;
	case lexer::token_id::final_:
		return memory::virtual_address::attribute::final_;
	case lexer::token_id::ref:
		return memory::virtual_address::attribute::ref;
	case lexer::token_id::private_:
		return memory::virtual_address::attribute::private_;
	case lexer::token_id::protected_:
		return memory::virtual_address::attribute::protected_;
	default:
		break;
	}

	return memory::virtual_address::attribute::nil;
}
