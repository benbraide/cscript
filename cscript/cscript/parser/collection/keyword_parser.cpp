#include "keyword_parser.h"
#include "../../common/env.h"

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse(){
	if (common::env::parser_info.token == nullptr || common::env::error.has())
		return nullptr;

	switch (common::env::source_info->rule.map_index(common::env::parser_info.token->get_match_index())){
	case lexer::token_id::typedef_:
		return parse_typedef_();
	case lexer::token_id::type_cast:
		return parse_type_cast_();
	case lexer::token_id::placeholder:
		return parse_placeholder_();
	case lexer::token_id::operator_:
		return parse_operator_();
	case lexer::token_id::array_:
		return parse_array_type_();
	case lexer::token_id::pointer:
		return parse_pointer_type_();
	case lexer::token_id::function:
		return parse_function_type_();
	default:
		break;
	}

	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse_typedef_(){
	//typedef <type> name
	auto index = common::env::parser_info.token->get_index();

	lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
	common::env::source_info->source.ignore(*common::env::source_info);

	auto type = common::env::builder.parse_type();
	if (common::env::error.has())
		return nullptr;

	if (type == nullptr || !type->is(node::id::type_compatible) || type->is(node::id::type_with_storage) ||
		type->is(node::id::auto_type) || type->is(node::id::variadic_type)){
		return common::env::error.set("", index);
	}

	auto next = common::env::source_info->source.peek(*common::env::source_info);
	if (next == nullptr)
		return common::env::error.set("", index);

	node_type id_node;
	auto id = common::env::source_info->rule.map_index(next->get_match_index());
	if (id == lexer::token_id::placeholder){
		id_node = parse_placeholder_();
		if (common::env::error.has())
			return nullptr;
	}
	else if (id == lexer::token_id::identifier){
		common::env::source_info->source.ignore(*common::env::source_info);
		id_node = std::make_shared<node::identifier>(next->get_index(), next->get_value());
	}
	else//Error
		return common::env::error.set("", index);

	return std::make_shared<node::type_definition>(index, type, id_node);
}

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse_type_cast_(){
	//type_cast< <type> >(<value>)
	auto index = common::env::parser_info.token->get_index();

	lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
	common::env::source_info->source.ignore(*common::env::source_info);

	auto type = parse_type_(index, true);
	if (common::env::error.has())
		return nullptr;

	auto value = parse_value_(index);
	if (common::env::error.has())
		return nullptr;

	return std::make_shared<node::type_cast>(index, type, value);
}

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse_placeholder_(){
	//__placeholder(<value>)
	auto index = common::env::parser_info.token->get_index();

	lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
	common::env::source_info->source.ignore(*common::env::source_info);

	auto value = parse_value_(index);
	if (common::env::error.has())
		return nullptr;

	return std::make_shared<node::placeholder>(index, value);
}

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse_operator_(){
	//operator [\left|\right] <symbol>|<type>
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse_array_type_(){
	//array< <type> >
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse_pointer_type_(){
	//pointer_t< <type> >
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse_function_type_(){
	//function< <return_type>(<parameter_types>) >
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse_type_(
	const lexer::token::index &index, bool allow_storage_class){
	auto next = common::env::source_info->source.next(*common::env::source_info);
	if (next == nullptr || common::env::source_info->rule.map_index(next->get_match_index()) != lexer::token_id::operator_symbol ||
		next->get_value() != "<"){
		return common::env::error.set("", index);
	}

	auto type = common::env::builder.parse_single(builder::halt_info{ lexer::token_id::operator_symbol, ">" });
	if (common::env::error.has())
		return nullptr;

	if (type == nullptr || !type->is(node::id::type_compatible))
		return common::env::error.set("", index);

	if (type->is(node::id::type_with_storage)){
		if (!allow_storage_class)
			return common::env::error.set("", index);

		auto attributes = type->query<node::type_with_storage_class>()->get_attributes();
		if (attributes != memory::virtual_address::attribute::ref && (attributes != (memory::virtual_address::attribute::ref |
			memory::virtual_address::attribute::constant))){
			return common::env::error.set("", index);
		}
	}

	return type;
}

cscript::parser::generic::node_type cscript::parser::collection::keyword::parse_value_(const lexer::token::index &index){
	auto next = common::env::source_info->source.next(*common::env::source_info);
	if (next == nullptr || common::env::source_info->rule.map_index(next->get_match_index()) != lexer::token_id::open_par)
		return common::env::error.set("", index);

	auto value = common::env::builder.parse_single(builder::halt_info{ lexer::token_id::close_par });
	if (common::env::error.has())
		return nullptr;

	if (value == nullptr)
		return common::env::error.set("", index);

	return value;
}
