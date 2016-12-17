#include "token_formatter.h"

cscript::lexer::formatter::linkable_token_formatter::linkable_token_formatter(const generic_token_formatter *next)
	: next_(next){}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::linkable_token_formatter::call_next_(
	match_info &match, source_info &info) const{
	return (next_ == nullptr) ? nullptr : next_->format(match, info);
}

cscript::lexer::formatter::forward_slash::forward_slash(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::forward_slash::format(match_info &match, source_info &info) const{
	/*
		// --> Single line comment
		/* --> Multiline comment
		/? --> Operator
	*/

	if (info.rule.map_index(match.match_index) == token_id::forward_slash)
		return format_(match, info);

	return call_next_(match, info);
}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::forward_slash::format_(match_info &match, source_info &info) const{
	auto next = info.source.peek(info);
	if (next == nullptr)
		return nullptr;

	auto id = info.rule.map_index(next->get_match_index());
	if (id == token_id::forward_slash){//Single line comment
		info.source.ignore(info);
		match.value.append(1, '/');
		match.adjustment.left = 2;

		while (info.source.has_more()){//Skip to end of line
			if (*match.value.append(1, info.source.get_char()).rbegin() == '\n'){
				if (*std::next(match.value.rbegin()) != '\\')//Not extended
					break;
			}
		}
	}
	else if (id == token_id::symbol && next->get_value()[0] == '*'){//Multiline comment
		info.source.ignore(info);
		match.value.append(1, '*');

		match.adjustment.left = 2;
		match.adjustment.right = 2;

		auto terminated = false;
		while (info.source.has_more()){//Skip to end of comment --> */
			if (*match.value.append(1, info.source.get_char()).rbegin() == '*'){//Check next
				if (info.source.has_more() && *match.value.append(1, info.source.get_char()).rbegin() == '/'){
					terminated = true;
					break;
				}
			}
		}

		if (!terminated)//Not terminated
			return std::make_shared<error_token>(match.index, "Comment is not terminated");
	}

	return std::make_shared<token>(match.index, match.value, match.match_index, match.adjustment);
}

cscript::lexer::formatter::string::string(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::string::format(match_info &match, source_info &info) const{
	switch (info.rule.map_index(match.match_index)){
	case token_id::quote_dbl:
	case token_id::quote_sng:
	case token_id::quote_back:
		return format_(match, info, false);
	case token_id::esc_quote_dbl:
	case token_id::esc_quote_sng:
	case token_id::esc_quote_back:
		return format_(match, info, true);
	default:
		break;
	}

	return call_next_(match, info);
}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::string::format_(match_info &match, source_info &info, bool escaped) const{
	auto terminated = false;
	char quote = *match.value.rbegin(), c;
	
	while (info.source.has_more()){
		if ((c = *match.value.append(1, info.source.get_char()).rbegin()) == quote){
			terminated = true;
			break;
		}

		if (escaped && c == '\\' && info.source.has_more())//Escaped
			match.value.append(1, info.source.get_char());
	}

	if (!terminated)//Not terminated
		return std::make_shared<error_token>(match.index, "String is not terminated");

	return std::make_shared<token>(match.index, match.value, match.match_index, match.adjustment);
}

cscript::lexer::formatter::type::type(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::type::format(match_info &match, source_info &info) const{
	switch (info.rule.map_index(match.match_index)){
	case token_id::unsigned_:
		return format_unsigned_(match, info);
	case token_id::long_:
		return format_long_(match, info);
	default:
		break;
	}

	return call_next_(match, info);
}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::type::format_unsigned_(match_info &match, source_info &info) const{
	std::string blanks;
	auto source_index = 1;

	auto next = next_(match, info, source_index, blanks);
	if (next == nullptr)
		return nullptr;

	auto is_long = false;
	switch (info.rule.map_index(next->get_match_index())){
	case token_id::char_://uchar
		match.match_index = static_cast<int>(token_id::uchar);
		break;
	case token_id::short_://ushort
		match.match_index = static_cast<int>(token_id::ushort);
		break;
	case token_id::int_://uint
		match.match_index = static_cast<int>(token_id::uint);
		break;
	case token_id::long_://ulong
		is_long = true;
		match.match_index = static_cast<int>(token_id::ulong);
		break;
	default:
		return std::make_shared<error_token>(match.index, "'unsigned' without target type");
	}

	match.value += (blanks + next->get_value());
	info.source.ignore(info, source_index);

	if (is_long)//Try ullong
		return format_unsigned_long_(match, info);

	return std::make_shared<token>(match.index, match.value, match.match_index, match.adjustment);
}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::type::format_unsigned_long_(match_info &match, source_info &info) const{
	std::string blanks;
	auto source_index = 1;

	auto next = next_(match, info, source_index, blanks);
	if (next == nullptr)
		return nullptr;

	if (info.rule.map_index(next->get_match_index()) == token_id::long_){
		match.match_index = static_cast<int>(token_id::ullong);
		match.value += (blanks + next->get_value());
		info.source.ignore(info, source_index);
	}

	return std::make_shared<token>(match.index, match.value, match.match_index, match.adjustment);
}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::type::format_long_(match_info &match, source_info &info) const{
	std::string blanks;
	auto source_index = 1;
	
	auto next = next_(match, info, source_index, blanks);
	if (next == nullptr)
		return nullptr;

	switch (info.rule.map_index(next->get_match_index())){
	case token_id::long_://llong
		match.match_index = static_cast<int>(token_id::llong);
		break;
	case token_id::double_://ldouble
		match.match_index = static_cast<int>(token_id::ldouble);
		break;
	default:
		return std::make_shared<token>(match.index, match.value, match.match_index, match.adjustment);
	}

	match.value += (blanks + next->get_value());
	info.source.ignore(info, source_index);

	return std::make_shared<token>(match.index, match.value, match.match_index, match.adjustment);
}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::type::next_(
	match_info &match, source_info &info, int &count, std::string &blanks) const{
	auto_info disable_all(info, nullptr, nullptr);
	token_type next = nullptr;

	while (info.source.has_more()){
		if ((next = info.source.peek(info, count)) == nullptr)
			return nullptr;

		if (token_id_compare_collection::skip.is(info.rule.map_index(next->get_match_index()))){
			blanks += next->get_value();
			next = nullptr;
			++count;
		}
		else//Stop
			break;
	}

	return next;
}

const cscript::lexer::formatter::forward_slash cscript::lexer::formatter::collection::forward_slash;

const cscript::lexer::formatter::string cscript::lexer::formatter::collection::string;

const cscript::lexer::formatter::type cscript::lexer::formatter::collection::type;

const cscript::lexer::formatter::forward_slash cscript::lexer::formatter::linked_collection::forward_slash;

const cscript::lexer::formatter::string cscript::lexer::formatter::linked_collection::string(&forward_slash);

const cscript::lexer::formatter::type cscript::lexer::formatter::linked_collection::type(&string);

const cscript::lexer::generic_token_formatter& cscript::lexer::formatter::linked_collection::last = type;
