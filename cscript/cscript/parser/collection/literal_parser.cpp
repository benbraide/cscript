#include "literal_parser.h"

cscript::parser::generic::node_type cscript::parser::collection::literal::parse(){
	auto id = common::env::source_info->rule.map_index(common::env::parser_info.token->get_match_index());
	if (id == lexer::token_id::operator_symbol){
		switch (common::env::parser_info.token->query<lexer::operator_token>()->get_id()){
		case lexer::operator_id::plus:
		case lexer::operator_id::minus:
			break;
		default:
			return nullptr;
		}

		lexer::auto_skip enable_skip(*common::env::source_info, &lexer::token_id_compare_collection::skip);
		auto next = common::env::source_info->source.peek(*common::env::source_info);
		if (next != nullptr && lexer::token_id_compare_collection::numeric.is(common::env::source_info->rule.map_index(next->get_match_index()))){
			common::env::source_info->source.ignore(*common::env::source_info);
			next->update(common::env::parser_info.token->get_value() + next->get_value());
		}
		else//Operator
			return nullptr;

		common::env::parser_info.token = next;//Signed number
		id = common::env::source_info->rule.map_index(next->get_match_index());
	}

	switch (id){
	case lexer::token_id::dec:
		return parse_dec_();
	case lexer::token_id::hex:
		return parse_hex_();
	case lexer::token_id::oct:
		return parse_oct_();
	case lexer::token_id::rad:
		return parse_rad_();
	case lexer::token_id::bin:
		return parse_bin_();
	case lexer::token_id::real:
	case lexer::token_id::exp:
		return parse_real_();
	case lexer::token_id::quote_sng:
		return parse_single_(false);
	case lexer::token_id::quote_dbl:
		return parse_double_(false);
	case lexer::token_id::quote_back:
		return parse_back_(false);
	case lexer::token_id::esc_quote_sng:
		return parse_single_(true);
	case lexer::token_id::esc_quote_dbl:
		return parse_double_(true);
	case lexer::token_id::esc_quote_back:
		return parse_back_(true);
	default:
		break;
	}

	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_dec_(){
	std::string suffix_string;
	auto suffix_value = get_integral_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, token_value, suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ullong_type, std::stoull(value));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::llong_type, std::stoll(value));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ulong_type, std::stoul(value));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::long_type, std::stol(value));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::uint_type, static_cast<unsigned int>(std::stoul(value)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::int_type, std::stoi(value));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_hex_(){
	std::string suffix_string;
	auto suffix_value = get_integral_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, token_value, suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ullong_type, std::stoull(value.substr(2), nullptr, 16));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::llong_type, std::stoll(value.substr(2), nullptr, 16));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ulong_type, std::stoul(value.substr(2), nullptr, 16));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::long_type, std::stol(value.substr(2), nullptr, 16));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::uint_type, static_cast<unsigned int>(std::stoul(value.substr(2), nullptr, 16)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::int_type, std::stoi(value.substr(2), nullptr, 16));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_oct_(){
	std::string suffix_string;
	auto suffix_value = get_integral_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, token_value, suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ullong_type, std::stoull(value, nullptr, 8));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::llong_type, std::stoll(value, nullptr, 8));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ulong_type, std::stoul(value, nullptr, 8));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::long_type, std::stol(value, nullptr, 8));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::uint_type, static_cast<unsigned int>(std::stoul(value, nullptr, 8)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::int_type, std::stoi(value, nullptr, 8));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_rad_(){
	std::string suffix_string;
	auto suffix_value = get_integral_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, token_value, suffix_string, [suffix_value](const std::string &value){
		std::string::size_type offset;
		auto base = std::stoi(value, &offset);

		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ullong_type, std::stoull(value.substr(offset + 1), nullptr, base));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::llong_type, std::stoll(value.substr(offset + 1), nullptr, base));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ulong_type, std::stoul(value.substr(offset + 1), nullptr, base));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::long_type, std::stol(value.substr(offset + 1), nullptr, base));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::uint_type, static_cast<unsigned int>(std::stoul(value.substr(offset + 1), nullptr, base)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::int_type, std::stoi(value.substr(offset + 1), nullptr, base));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_bin_(){
	std::string suffix_string;
	auto suffix_value = get_integral_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, token_value, suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ullong_type, std::stoull(value.substr(2), nullptr, 2));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::llong_type, std::stoll(value.substr(2), nullptr, 2));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ulong_type, std::stoul(value.substr(2), nullptr, 2));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::long_type, std::stol(value.substr(2), nullptr, 2));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::uint_type, static_cast<unsigned int>(std::stoul(value.substr(2), nullptr, 2)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::int_type, std::stoi(value.substr(2), nullptr, 2));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_real_(){
	std::string suffix_string;
	auto suffix_value = get_real_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, token_value, suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::float_:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::float_type, std::stof(value));
		case suffix::ldouble:
			return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::ldouble_type, std::stold(value));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(common::env::temp_address_space, common::env::double_type, std::stod(value));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_single_(bool escaped){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_double_(bool escaped){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_back_(bool escaped){
	return nullptr;
}

cscript::parser::collection::literal::suffix cscript::parser::collection::literal::get_integral_suffix_(std::string &value){
	static boost::regex regex("^([uU][lL][lL])|([uU][lL])|([lL][lL])|([uU])|([lL])$", boost::regex_constants::optimize);

	lexer::auto_skip disable_skip(*common::env::source_info, nullptr);
	auto next = common::env::source_info->source.peek(*common::env::source_info);
	if (next == nullptr || common::env::source_info->rule.map_index(next->get_match_index()) != lexer::token_id::identifier)
		return suffix::nil;

	value = next->get_value();
	boost::match_results<std::string::const_iterator> results;
	if (!boost::regex_search(value.cbegin(), value.cend(), results, regex, boost::regex_constants::match_single_line)){
		common::env::error.set("Invalid suffix", next->get_index());
		return suffix::nil;
	}

	switch (get_match_index_(results)){
	case 0:
		return suffix::ullong;
	case 1:
		return suffix::ulong;
	case 2:
		return suffix::llong;
	case 3:
		return suffix::uint;
	default:
		break;
	}

	return suffix::long_;
}

cscript::parser::collection::literal::suffix cscript::parser::collection::literal::get_real_suffix_(std::string &value){
	lexer::auto_skip disable_skip(*common::env::source_info, nullptr);
	auto next = common::env::source_info->source.peek(*common::env::source_info);
	if (next == nullptr || common::env::source_info->rule.map_index(next->get_match_index()) != lexer::token_id::identifier)
		return suffix::nil;

	value = next->get_value();
	if (value.size() != 1u){
		common::env::error.set("Invalid suffix", next->get_index());
		return suffix::nil;
	}

	switch (tolower(value[0])){
	case 'f':
		return suffix::float_;
	case 'l':
		return suffix::ldouble;
	default:
		break;
	}

	common::env::parser_info.token->update(next->get_index(), "Invalid suffix", common::env::source_info->rule.get_error_index());
	return suffix::nil;
}

cscript::parser::collection::literal::suffix cscript::parser::collection::literal::get_string_suffix_(std::string &value){
	lexer::auto_skip disable_skip(*common::env::source_info, nullptr);
	auto next = common::env::source_info->source.peek(*common::env::source_info);
	if (next == nullptr || common::env::source_info->rule.map_index(next->get_match_index()) != lexer::token_id::identifier)
		return suffix::nil;

	value = next->get_value();
	if (value.size() != 1u || tolower(value[0]) != 'w'){
		common::env::error.set("Invalid suffix", next->get_index());
		return suffix::nil;
	}

	return suffix::wchar;
}

int cscript::parser::collection::literal::get_match_index_(boost::match_results<std::string::const_iterator> &results){
	auto index = 0;
	for (auto match = std::next(results.begin()); match != results.end(); ++match, ++index){
		if (match->matched)
			break;
	}

	return index;
}