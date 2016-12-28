#include "token_formatter.h"

cscript::lexer::formatter::linkable_token_formatter::linkable_token_formatter(const generic_token_formatter *next)
	: next_(next){}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::linkable_token_formatter::call_next_(
	match_info &match, source_info &info) const{
	return (next_ == nullptr) ? nullptr : next_->format(match, info);
}

cscript::lexer::formatter::comment::comment(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::comment::format(
	match_info &match, source_info &info) const{
	switch (info.rule.map_index(match.match_index)){
	case token_id::comment_sng:
		return format_single_line_(match, info);
	case token_id::comment_mult:
		return format_multiline_(match, info);
	default:
		break;
	}

	return call_next_(match, info);
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::comment::format_single_line_(
	match_info &match, source_info &info) const{
	match.adjustment.left = 2;

	while (info.source.has_more()){//Skip to end of line
		if (info.source.get_char() == '\n'){
			if (*(match.start + (match.size - 1)) != '\\')//Not extended
				break;
		}

		++match.size;
	}

	return nullptr;//Default creation
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::comment::format_multiline_(
	match_info &match, source_info &info) const{
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

cscript::lexer::formatter::string::string(const generic_token_formatter *next)
	: linkable_token_formatter(next){}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::string::format(
	match_info &match, source_info &info) const{
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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::string::format_(
	match_info &match, source_info &info, bool escaped) const{
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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::type::format(
	match_info &match, source_info &info) const{
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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::type::format_unsigned_(
	match_info &match, source_info &info) const{
	auto_info disable_all(info, nullptr, nullptr);

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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::type::format_unsigned_long_(
	match_info &match, source_info &info) const{
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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::type::format_long_(
	match_info &match, source_info &info) const{
	auto_info disable_all(info, nullptr, nullptr);

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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::operator_symbol::format(
	match_info &match, source_info &info) const{
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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format(
	match_info &match, source_info &info) const{
	switch (info.rule.map_index(match.match_index)){
	case token_id::prep_incl:
		return format_include_(match, info);
	case token_id::prep_def:
		return format_define_(match, info);
	case token_id::prep_ifdef:
		return format_if_defined_(match, info, true);
	case token_id::prep_ifndef:
		return format_if_defined_(match, info, false);
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

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_include_(
	match_info &match, source_info &info) const{
	auto_info enable_skip_restrict_format(info, &token_id_compare_collection::single_line_skip, &collection::comment);

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
		if (next->get_value().size() == 1u || next->get_value()[0] != '<'){
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

	auto error = check_for_new_line_(match, info);
	if (error != nullptr)
		return error;

	return [is_absolute, path](match_info &match){ return std::make_shared<preprocessor_token::include>(
		match.index, path, match.match_index, is_absolute); };
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_define_(
	match_info &match, source_info &info) const{
	token_type next, token;
	{//Scope
		auto_info enable_skip_restrict_format(info, &token_id_compare_collection::single_line_skip, &collection::comment, generic_source::option::no_expansion);
		next = info.source.next(info);
		if (next == nullptr){
			match.match_index = info.rule.get_error_index();
			return [](match_info &match){ return std::make_shared<error_token>(match.index, "Missing target name"); };
		}

		if (info.rule.map_index(next->get_match_index()) != token_id::identifier){
			match.match_index = info.rule.get_error_index();
			return [](match_info &match){ return std::make_shared<error_token>(match.index, "Missing target name"); };
		}
	}

	token_id id;
	auto_skip disable_skip(info, nullptr);
	auto token_list = std::make_shared<preprocessor_token::define::list_type>();

	while (info.source.has_more()){
		if ((token = info.source.next(info)) != nullptr){
			id = info.rule.map_index(token->get_match_index());
			if (token_id_compare_collection::preprocessor.is(id)){
				match.match_index = info.rule.get_error_index();
				return [token](match_info &match){ return std::make_shared<error_token>(token->get_index(), "Invalid token"); };
			}

			if (id == token_id::new_line){
				if (!token_list->empty() && info.rule.map_index((*token_list->rbegin())->get_match_index()) == token_id::backslash)
					token_list->pop_back();//Extended
				else//End of definition
					break;
			}
			else//Add to list
				token_list->push_back(token);
		}
		else//Invalid
			break;
	}

	if (!token_list->empty() && info.rule.map_index((*token_list->rbegin())->get_match_index()) == token_id::backslash){
		match.match_index = info.rule.get_error_index();
		return [](match_info &match){ return std::make_shared<error_token>(match.index, "Expected more tokens after '\\'"); };
	}

	return [next, token_list](match_info &match){ return std::make_shared<preprocessor_token::define>(
		match.index, next->get_value(), match.match_index, std::move(*token_list)); };
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_undefine_(
	match_info &match, source_info &info) const{
	token_type target;
	auto error = get_target_(match, info, target);
	if (error != nullptr)
		return error;

	return [target](match_info &match){ return std::make_shared<preprocessor_token::undefine>(
		match.index, target->get_value(), match.match_index); };
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_if_defined_(
	match_info &match, source_info &info, bool is_defined) const{
	auto_skip save_skipper(info, info.skipper);

	token_type target;
	auto error = get_target_(match, info, target);
	if (error != nullptr)
		return error;

	auto states = preprocessor_token::conditional::state::nil;
	if (info.symbols.is_defined(target->get_value()) != is_defined)
		CSCRIPT_SET(states, preprocessor_token::conditional::state::is_rejected);

	return [states](match_info &match){ return std::make_shared<preprocessor_token::conditional>(
		match.index, std::string(match.start, match.size), match.match_index, states); };
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_else_(
	match_info &match, source_info &info) const{
	token_type target;
	{//Scope
		auto_skip save_skipper(info, info.skipper);
		auto error = check_for_new_line_(match, info);
		if (error != nullptr)
			return error;
	}

	return [](match_info &match){ return std::make_shared<preprocessor_token::conditional>(
		match.index, "else", match.match_index, preprocessor_token::conditional::state::is_else); };
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_end_if_(
	match_info &match, source_info &info) const{
	{//Scope
		auto_skip save_skipper(info, info.skipper);
		auto error = check_for_new_line_(match, info);
		if (error != nullptr)
			return error;
	}

	return nullptr;
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::format_pragma_(
	match_info &match, source_info &info) const{
	return nullptr;
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::check_for_new_line_(
	match_info &match, source_info &info) const{
	token_id id;
	token_type next;

	info.skipper = nullptr;//Disable skip
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

	return nullptr;
}

cscript::lexer::generic_token_formatter::creator_type cscript::lexer::formatter::preprocessor::get_target_(
	match_info &match, source_info &info, token_type &target) const{
	auto_info enable_skip_restrict_format(info, &token_id_compare_collection::single_line_skip, &collection::comment, generic_source::option::no_expansion);
	if ((target = info.source.next(info)) == nullptr){
		match.match_index = info.rule.get_error_index();
		return [](match_info &match){ return std::make_shared<error_token>(match.index, "Missing target name"); };
	}

	if (info.rule.map_index(target->get_match_index()) != token_id::identifier){
		match.match_index = info.rule.get_error_index();
		return [](match_info &match){ return std::make_shared<error_token>(match.index, "Missing target name"); };
	}

	return check_for_new_line_(match, info);
}

const cscript::lexer::operator_symbol cscript::lexer::formatter::operator_symbol::symbols;

const cscript::lexer::formatter::comment cscript::lexer::formatter::collection::comment;

const cscript::lexer::formatter::string cscript::lexer::formatter::collection::string;

const cscript::lexer::formatter::type cscript::lexer::formatter::collection::type;

const cscript::lexer::formatter::operator_symbol cscript::lexer::formatter::collection::operator_symbol;

const cscript::lexer::formatter::preprocessor cscript::lexer::formatter::collection::preprocessor;

const cscript::lexer::formatter::operator_symbol cscript::lexer::formatter::linked_collection::operator_symbol;

const cscript::lexer::formatter::comment cscript::lexer::formatter::linked_collection::comment(&operator_symbol);

const cscript::lexer::formatter::string cscript::lexer::formatter::linked_collection::string(&comment);

const cscript::lexer::formatter::type cscript::lexer::formatter::linked_collection::type(&string);

const cscript::lexer::formatter::preprocessor cscript::lexer::formatter::linked_collection::preprocessor(&type);

const cscript::lexer::generic_token_formatter& cscript::lexer::formatter::linked_collection::last = preprocessor;
