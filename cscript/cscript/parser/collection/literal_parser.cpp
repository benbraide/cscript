#include "literal_parser.h"
#include "../../object/numeric_object.h"

cscript::parser::generic::node_type cscript::parser::collection::literal::parse(){
	if (common::env::parser_info.token == nullptr || common::env::error.has())
		return nullptr;

	auto id = common::env::source_info.rule->map_index(common::env::parser_info.token->get_match_index());
	if (id == lexer::token_id::operator_symbol){
		switch (common::env::parser_info.token->query<lexer::operator_token>()->get_id()){
		case lexer::operator_id::plus:
		case lexer::operator_id::minus:
			break;
		default:
			return nullptr;
		}

		lexer::auto_skip enable_skip(common::env::source_info, &lexer::token_id_compare_collection::skip);
		auto next = common::env::source_info.source->peek(common::env::source_info);
		if (next != nullptr && lexer::token_id_compare_collection::numeric.is(common::env::source_info.rule->map_index(next->get_match_index()))){
			common::env::source_info.source->ignore(common::env::source_info);
			next->update(common::env::parser_info.token->get_value() + next->get_value());
		}
		else//Operator
			return nullptr;

		common::env::parser_info.token = next;//Signed number
		id = common::env::source_info.rule->map_index(next->get_match_index());
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

	return std::make_shared<node::literal>(index, lexer::token_id::dec, token_value + suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(common::env::ullong_type, std::stoull(value));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(common::env::llong_type, std::stoll(value));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(common::env::ulong_type, std::stoul(value));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(common::env::long_type, std::stol(value));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(common::env::uint_type, static_cast<unsigned int>(std::stoul(value)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(common::env::int_type, std::stoi(value));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_hex_(){
	std::string suffix_string;
	auto suffix_value = get_integral_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, lexer::token_id::hex, token_value + suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(
				common::env::ullong_type, std::stoull(value.substr(2), nullptr, 16));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(
				common::env::llong_type, std::stoll(value.substr(2), nullptr, 16));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(
				common::env::ulong_type, std::stoul(value.substr(2), nullptr, 16));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(
				common::env::long_type, std::stol(value.substr(2), nullptr, 16));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(
				common::env::uint_type, static_cast<unsigned int>(std::stoul(value.substr(2), nullptr, 16)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(
			common::env::int_type, std::stoi(value.substr(2), nullptr, 16));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_oct_(){
	std::string suffix_string;
	auto suffix_value = get_integral_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, lexer::token_id::oct, token_value + suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(
				common::env::ullong_type, std::stoull(value, nullptr, 8));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(
				common::env::llong_type, std::stoll(value, nullptr, 8));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(
				common::env::ulong_type, std::stoul(value, nullptr, 8));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(
				common::env::long_type, std::stol(value, nullptr, 8));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(
				common::env::uint_type, static_cast<unsigned int>(std::stoul(value, nullptr, 8)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(common::env::int_type, std::stoi(value, nullptr, 8));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_rad_(){
	std::string suffix_string;
	auto suffix_value = get_integral_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, lexer::token_id::rad, token_value + suffix_string, [suffix_value](const std::string &value){
		std::string::size_type offset;
		auto base = std::stoi(value, &offset);

		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(
				common::env::ullong_type, std::stoull(value.substr(offset + 1), nullptr, base));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(
				common::env::llong_type, std::stoll(value.substr(offset + 1), nullptr, base));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(
				common::env::ulong_type, std::stoul(value.substr(offset + 1), nullptr, base));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(
				common::env::long_type, std::stol(value.substr(offset + 1), nullptr, base));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(
				common::env::uint_type, static_cast<unsigned int>(std::stoul(value.substr(offset + 1), nullptr, base)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(
			common::env::int_type, std::stoi(value.substr(offset + 1), nullptr, base));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_bin_(){
	std::string suffix_string;
	auto suffix_value = get_integral_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();

	return std::make_shared<node::literal>(index, lexer::token_id::bin, token_value + suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::ullong:
			return std::make_shared<object::primitive::numeric>(
				common::env::ullong_type, std::stoull(value.substr(2), nullptr, 2));
		case suffix::llong:
			return std::make_shared<object::primitive::numeric>(
				common::env::llong_type, std::stoll(value.substr(2), nullptr, 2));
		case suffix::ulong:
			return std::make_shared<object::primitive::numeric>(
				common::env::ulong_type, std::stoul(value.substr(2), nullptr, 2));
		case suffix::long_:
			return std::make_shared<object::primitive::numeric>(
				common::env::long_type, std::stol(value.substr(2), nullptr, 2));
		case suffix::uint:
			return std::make_shared<object::primitive::numeric>(
				common::env::uint_type, static_cast<unsigned int>(std::stoul(value.substr(2), nullptr, 2)));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(
			common::env::int_type, std::stoi(value.substr(2), nullptr, 2));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_real_(){
	std::string suffix_string;
	auto suffix_value = get_real_suffix_(suffix_string);
	if (common::env::error.has())//Error
		return nullptr;

	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();
	auto id = common::env::source_info.rule->map_index(common::env::parser_info.token->get_match_index());

	return std::make_shared<node::literal>(index, id, token_value + suffix_string, [suffix_value](const std::string &value){
		switch (suffix_value){
		case suffix::float_:
			return std::make_shared<object::primitive::numeric>(common::env::float_type, std::stof(value));
		case suffix::ldouble:
			return std::make_shared<object::primitive::numeric>(common::env::ldouble_type, std::stold(value));
		default:
			break;
		}

		return std::make_shared<object::primitive::numeric>(common::env::double_type, std::stod(value));
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_single_(bool escaped){
	return nullptr;
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_double_(bool escaped){
	auto &token_value = common::env::parser_info.token->get_value();
	auto &index = common::env::parser_info.token->get_index();
	auto id = lexer::token_id::quote_dbl;

	return std::make_shared<node::literal>(index, id, token_value, [escaped](const std::string &value){
		std::string value_copy(std::next(value.begin()), std::prev(value.end()));
		if (escaped && !value_copy.empty())
			escape_string_(value_copy);

		return common::env::create_string(value_copy);
	});
}

cscript::parser::generic::node_type cscript::parser::collection::literal::parse_back_(bool escaped){
	return nullptr;
}

cscript::parser::collection::literal::suffix cscript::parser::collection::literal::get_integral_suffix_(std::string &value){
	static boost::regex regex("^([uU][lL][lL])|([uU][lL])|([lL][lL])|([uU])|([lL])$", boost::regex_constants::optimize);

	lexer::auto_skip disable_skip(common::env::source_info, nullptr);
	auto next = common::env::source_info.source->peek_after(common::env::parser_info.token, common::env::source_info);
	if (next == nullptr || common::env::source_info.rule->map_index(next->get_match_index()) != lexer::token_id::identifier)
		return suffix::nil;

	value = next->get_value();
	common::env::source_info.source->ignore_one(next);

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
	lexer::auto_skip disable_skip(common::env::source_info, nullptr);
	auto next = common::env::source_info.source->peek_after(common::env::parser_info.token, common::env::source_info);
	if (next == nullptr || common::env::source_info.rule->map_index(next->get_match_index()) != lexer::token_id::identifier)
		return suffix::nil;

	value = next->get_value();
	common::env::source_info.source->ignore_one(next);

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

	common::env::parser_info.token->update(next->get_index(), "Invalid suffix", common::env::source_info.rule->get_error_index());
	return suffix::nil;
}

cscript::parser::collection::literal::suffix cscript::parser::collection::literal::get_string_suffix_(std::string &value){
	lexer::auto_skip disable_skip(common::env::source_info, nullptr);
	auto next = common::env::source_info.source->peek(common::env::source_info);
	if (next == nullptr || common::env::source_info.rule->map_index(next->get_match_index()) != lexer::token_id::identifier)
		return suffix::nil;

	value = next->get_value();
	if (value.size() != 1u || tolower(value[0]) != 'w'){
		common::env::error.set("Invalid suffix", next->get_index());
		return suffix::nil;
	}

	return suffix::wchar;
}

void cscript::parser::collection::literal::escape_string_(std::string &value){
	static boost::regex regex(get_escapes_(), boost::regex_constants::optimize);
	value = boost::regex_replace(value, regex, [](const boost::smatch &match) -> std::string{
		switch (get_match_index_(match)){
		case 0:
			return "\0";
		case 1:
			return "\a";
		case 2:
			return "\b";
		case 3:
			return "\f";
		case 4:
			return "\n";
		case 5:
			return "\r";
		case 6:
			return "\t";
		case 7:
			return "\v";
		case 8:
			return "\"";
		case 9:
			return "\'";
		case 10:
			return "`";
		case 11:
			return "\\";
		case 12://Rad
			return "\\";
		case 13://Hex
			return std::string(1, static_cast<char>(std::stoi(std::string(std::next(match[13].begin(), 2), match[13].end()), nullptr, 16)));
		case 14://Oct
			return std::string(1, static_cast<char>(std::stoi(std::string(std::next(match[14].begin()), match[14].end()), nullptr, 8)));
		default://Unknown
			break;
		}

		return std::string(std::next(match[15].begin()), match[15].end());
	});
}

std::string cscript::parser::collection::literal::get_escapes_(){
	std::vector<std::string> escapes({
		"\\\\0",
		"\\\\a",
		"\\\\b",
		"\\\\f",
		"\\\\n",
		"\\\\r",
		"\\\\t",
		"\\\\v",
		"\\\\\"",
		"\\\\\'",
		"\\\\`",
		"\\\\\\\\",
		"\\\\((2[0-6])|(1[0-9])|[2-9])r",
		"\\\\x[0-9a-fA-F]+",
		"\\\\[1-7][0-7]*",
		"\\\\."
	});

	std::string value;
	for (auto &escape : escapes){
		if (value.empty())
			value = ("(" + escape + ")");
		else//Append
			value += ("|(" + escape + ")");
	}

	return value;
}

int cscript::parser::collection::literal::get_match_index_(const boost::match_results<std::string::const_iterator> &results){
	auto index = 0;
	for (auto match = std::next(results.begin()); match != results.end(); ++match, ++index){
		if (match->matched)
			break;
	}

	return index;
}
