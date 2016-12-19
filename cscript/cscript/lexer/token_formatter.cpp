#include "token_formatter.h"

cscript::lexer::formatter::linkable_token_formatter::linkable_token_formatter(const generic_token_formatter *next)
	: next_(next){}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::linkable_token_formatter::call_next_(
	match_info &match, source_info &info) const{
	return (next_ == nullptr) ? nullptr : next_->format(match, info);
}

cscript::lexer::formatter::forward_slash::forward_slash(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::forward_slash::format(match_info &match, source_info &info) const{
	/*
		// --> Single line comment
		/* --> Multiline comment
		/? --> Operator
	*/

	if (info.rule.map_index(match.match_index) == token_id::forward_slash)
		return format_(match, info);

	return call_next_(match, info);
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::forward_slash::format_(match_info &match, source_info &info) const{
	auto_info disable_all(info, nullptr, nullptr);

	auto next = info.source.peek(info);
	if (next == nullptr)
		return nullptr;

	auto id = info.rule.map_index(next->get_match_index());
	if (id == token_id::forward_slash){//Single line comment
		info.source.ignore(info);

		match.match_index = static_cast<int>(token_id::comment_sng);
		match.adjustment.left = 2;

		++match.size;
		while (info.source.has_more()){//Skip to end of line
			if (info.source.get_char() == '\n'){
				if (*(match.start + (match.size - 1)) != '\\')//Not extended
					break;
			}

			++match.size;
		}

		return nullptr;//Default creation
	}
	
	if (id == token_id::symbol && next->get_value()[0] == '*'){//Multiline comment
		info.source.ignore(info);
		++match.size;

		match.match_index = static_cast<int>(token_id::comment_mult);
		match.adjustment.left = 2;
		match.adjustment.right = 2;

		auto terminated = false;
		while (info.source.has_more()){//Skip to end of comment --> */
			if (info.source.get_char() == '*'){//Check next
				++match.size;
				if (info.source.has_more() && info.source.get_char() == '/'){
					++match.size;
					terminated = true;
					break;
				}
				else//Continue extraction
					++match.size;;
			}
			else//Continue extraction
				++match.size;
		}

		if (!terminated){//Not terminated
			match.match_index = info.rule.get_error_index();
			return [](match_info &match){ return std::make_shared<error_token>(match.index, "Comment is not terminated"); };
		}

		return nullptr;//Default creation
	}
	
	if (id == token_id::symbol)//Append symbol
		match.size += next->get_value().size();

	return call_next_(match, info);
}

cscript::lexer::formatter::string::string(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::string::format(match_info &match, source_info &info) const{
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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::string::format_(match_info &match, source_info &info, bool escaped) const{
	auto terminated = false;
	char quote = escaped ? *match.start : *(match.start + 1), c;

	match.adjustment.left = escaped ? 1 : 2;
	match.adjustment.right = 1;
	
	while (info.source.has_more()){
		if ((c = info.source.get_char()) == quote){
			terminated = true;
			++match.size;
			break;
		}

		if (escaped && c == '\\' && info.source.has_more()){//Escaped
			info.source.get_char();
			++match.size;
		}

		++match.size;
	}

	if (!terminated){//Not terminated
		match.match_index = info.rule.get_error_index();
		return [](match_info &match){ return std::make_shared<error_token>(match.index, "String is not terminated"); };
	}

	return nullptr;//Default creation
}

cscript::lexer::formatter::type::type(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::type::format(match_info &match, source_info &info) const{
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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::type::format_unsigned_(match_info &match, source_info &info) const{
	size_type blanks = 0;
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
		match.match_index = info.rule.get_error_index();
		return [](match_info &match){ return std::make_shared<error_token>(match.index, "'unsigned' without target type"); };
	}

	match.size += (blanks + next->get_value().size());
	info.source.ignore(info, source_index);

	if (is_long)//Try ullong
		return format_unsigned_long_(match, info);

	return nullptr;//Default creation
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::type::format_unsigned_long_(match_info &match, source_info &info) const{
	size_type blanks = 0;
	auto source_index = 1;

	auto next = next_(match, info, source_index, blanks);
	if (next == nullptr)
		return nullptr;

	if (info.rule.map_index(next->get_match_index()) == token_id::long_){
		match.match_index = static_cast<int>(token_id::ullong);
		match.size += (blanks + next->get_value().size());
		info.source.ignore(info, source_index);
	}

	return nullptr;//Default creation
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::type::format_long_(match_info &match, source_info &info) const{
	size_type blanks = 0;
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
		return nullptr;//Default creation
	}

	match.size += (blanks + next->get_value().size());
	info.source.ignore(info, source_index);

	return nullptr;//Default creation
}

cscript::lexer::generic_token_formatter::token_type cscript::lexer::formatter::type::next_(
	match_info &match, source_info &info, int &count, size_type &blanks) const{
	auto_info disable_all(info, nullptr, nullptr);
	token_type next = nullptr;

	while (info.source.has_more()){
		if ((next = info.source.peek(info, count)) == nullptr)
			return nullptr;

		if (token_id_compare_collection::skip.is(info.rule.map_index(next->get_match_index()))){
			blanks += next->get_value().size();
			next = nullptr;
			++count;
		}
		else//Stop
			break;
	}

	return next;
}

cscript::lexer::formatter::operator_symbol::operator_symbol(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::operator_symbol::format(match_info &match, source_info &info) const{
	operator_id id;
	auto tid = info.rule.map_index(match.match_index);
	if (tid == token_id::symbol)
		id = symbols.convert_id(std::string(match.start, match.size), token_id::symbol);
	else//Not a symbol
		id = symbols.convert_id("", tid);

	return (id == operator_id::nil) ? call_next_(match, info) : [id](match_info &match){
		return std::make_shared<operator_token>(match.index, std::string(match.start, match.size), id);
	};
}

cscript::lexer::formatter::preprocessor::preprocessor(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format(match_info &match, source_info &info) const{
	switch (info.rule.map_index(match.match_index)){
	case token_id::prep_incl:
		return format_include_(match, info);
	case token_id::prep_def:
		return format_define_(match, info);
	case token_id::prep_ifdef:
		return format_if_defined_(match, info);
	case token_id::prep_ifndef:
		return format_if_not_defined_(match, info);
	case token_id::prep_else:
		return format_else_(match, info);
	case token_id::prep_endif:
		return format_end_if_(match, info);
	case token_id::prep_pragma:
		return format_pragma_(match, info);
	default:
		break;
	}

	return call_next_(match, info);
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_include_(match_info &match, source_info &info) const{
	auto_info enable_skip_restrict_format(info, &token_id_compare_collection::skip, &collection::forward_slash);

	auto next = info.source.next(info);
	if (next == nullptr){
		match.match_index = info.rule.get_error_index();
		return [](match_info &match){ return std::make_shared<error_token>(match.index, "Missing target file path"); };
	}

	bool is_absolute;
	switch (info.rule.map_index(next->get_match_index())){
	case token_id::esc_quote_dbl:
		is_absolute = false;
		break;
	case token_id::symbol:
		if (next->get_value().empty() || next->get_value()[0] != '<'){
			match.match_index = info.rule.get_error_index();
			return [](match_info &match){ return std::make_shared<error_token>(match.index, "Missing target file path"); };
		}
		break;
	default:
		match.match_index = info.rule.get_error_index();
		return [](match_info &match){ return std::make_shared<error_token>(match.index, "Missing target file path"); };
	}

	std::string path;
	char close = is_absolute ? '<' : '"', c;

	while (info.source.has_more() && (c = info.source.get_char()) != close)
		path.append(1, c);

	info.skipper = nullptr;//Disable skip
	token_id id;

	while (info.source.has_more()){//Check for end of line
		next = info.source.next(info);
		if (next == nullptr)
			break;

		if (!token_id_compare_collection::skip.is(id = info.rule.map_index(next->get_match_index()))){
			match.match_index = info.rule.get_error_index();
			return [next](match_info &match){ return std::make_shared<error_token>(next->get_index(), "Invalid token"); };
		}

		if (id == token_id::new_line)
			break;
	}

	return [is_absolute, path](match_info &match){ return std::make_shared<preprocessor_token::include>(
		match.index, path, match.match_index, is_absolute); };
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_define_(match_info &match, source_info &info) const{
	return nullptr;
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_if_defined_(match_info &match, source_info &info) const{
	return nullptr;
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_if_not_defined_(match_info &match, source_info &info) const{
	return nullptr;
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_else_(match_info &match, source_info &info) const{
	return nullptr;
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_end_if_(match_info &match, source_info &info) const{
	return nullptr;
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_pragma_(match_info &match, source_info &info) const{
	return nullptr;
}

const cscript::lexer::operator_symbol cscript::lexer::formatter::operator_symbol::symbols;

const cscript::lexer::formatter::forward_slash cscript::lexer::formatter::collection::forward_slash;

const cscript::lexer::formatter::string cscript::lexer::formatter::collection::string;

const cscript::lexer::formatter::type cscript::lexer::formatter::collection::type;

const cscript::lexer::formatter::operator_symbol cscript::lexer::formatter::collection::operator_symbol;

const cscript::lexer::formatter::preprocessor cscript::lexer::formatter::collection::preprocessor;

const cscript::lexer::formatter::operator_symbol cscript::lexer::formatter::linked_collection::operator_symbol;

const cscript::lexer::formatter::forward_slash cscript::lexer::formatter::linked_collection::forward_slash(&operator_symbol);

const cscript::lexer::formatter::string cscript::lexer::formatter::linked_collection::string(&forward_slash);

const cscript::lexer::formatter::type cscript::lexer::formatter::linked_collection::type(&string);

const cscript::lexer::formatter::preprocessor cscript::lexer::formatter::linked_collection::preprocessor;

const cscript::lexer::generic_token_formatter& cscript::lexer::formatter::linked_collection::last = preprocessor;
