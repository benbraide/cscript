#include "token_formatter.h"

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::forward_slash::format(match_info &match, source_info &info){
	/*
		// --> Single line comment
		/* --> Multiline comment
		/? --> Operator
	*/

	if (info.rule.map_index(match.match_index) != token_id::forward_slash)
		return nullptr;

	return format_(match, info);
}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::forward_slash::format_(match_info &match, source_info &info){
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

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::string::format(match_info &match, source_info &info){
	auto value = format_(match, info);
	return (value == nullptr) ? forward_slash::format(match, info) : value;
}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::string::format_(match_info &match, source_info &info){
	bool escaped;
	switch (info.rule.map_index(match.match_index)){
	case token_id::quote_dbl:
	case token_id::quote_sng:
	case token_id::quote_back:
		escaped = false;
		break;
	case token_id::esc_quote_dbl:
	case token_id::esc_quote_sng:
	case token_id::esc_quote_back:
		escaped = true;
		break;
	default:
		return nullptr;
	}

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
