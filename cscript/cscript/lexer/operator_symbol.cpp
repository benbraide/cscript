#include "operator_symbol.h"

cscript::lexer::operator_symbol::operator_symbol(){
	std::vector<std::string> rules({
		"[.]{4,}", "===", "!==", "<<=", ">>=", "\\.\\.\\.", "[.]{3}", "\\|\\|", "&&", "==", "!=", "<=", ">=", "\\+=", "\\-=",
		"\\*=", "\\/=", "%=", "\\|=", "&=", "\\^=", "::", "\\->", "\\[\\]", "\\(\\)", "\\+\\+", "\\-\\-", "<<", ">>", "[.]{2}",
		"\\?", "\\|", "\\^", "&", "<", ">", "\\+", "\\-", "\\*", "\\/", "%", "\\.", "!", "~", "=", "[.]{1}"
	});

	std::string combined_value;
	for (auto &value : rules){
		if (combined_value.empty())
			combined_value = "(" + value + ")";
		else//Append
			combined_value += "|(" + value + ")";
	}

	compiled_.assign("^" + combined_value + "$", boost::regex_constants::optimize);
}

cscript::lexer::operator_id cscript::lexer::operator_symbol::match(const std::string &value) const{
	boost::match_results<std::string::const_iterator> results;
	if (!boost::regex_search(value.begin(), value.end(), results, compiled_, boost::regex_constants::match_single_line))
		return operator_id::nil;

	auto index = get_match_index_(results);
	return (static_cast<size_type>(index) < map_.size()) ? map_[index] : operator_id::nil;
}

cscript::lexer::operator_id cscript::lexer::operator_symbol::convert_id(const std::string &value, token_id id) const{
	switch (id){
	case token_id::comma:
		return operator_id::comma;
	case token_id::throw_:
		return operator_id::throw_;
	case token_id::delete_:
		return operator_id::delete_;
	case token_id::new_:
		return operator_id::new_;
	case token_id::sizeof_:
		return operator_id::sizeof_;
	case token_id::typename_:
		return operator_id::typename_;
	case token_id::typeof:
		return operator_id::typeof_;
	case token_id::symbol:
		return match(value);
	default:
		break;
	}

	return operator_id::nil;
}

int cscript::lexer::operator_symbol::get_match_index_(boost::match_results<std::string::const_iterator> &results) const{
	auto index = 0;
	for (auto match = std::next(results.begin()); match != results.end(); ++match, ++index){
		if (match->matched)
			break;
	}

	return index;
}

const cscript::lexer::operator_symbol::list_type cscript::lexer::operator_symbol::map_({
	operator_id::unknown,
	operator_id::explicit_equality,
	operator_id::explicit_inverse_equality,
	operator_id::compound_left_shift,
	operator_id::compound_right_shift,
	operator_id::ellipsis,
	operator_id::unknown,
	operator_id::relational_or,
	operator_id::relational_and,
	operator_id::equality,
	operator_id::inverse_equality,
	operator_id::less_or_equal,
	operator_id::more_or_equal,
	operator_id::compound_plus,
	operator_id::compound_minus,
	operator_id::compound_times,
	operator_id::compound_divide,
	operator_id::compound_modulus,
	operator_id::compound_bitwise_or,
	operator_id::compound_bitwise_and,
	operator_id::compound_bitwise_xor,
	operator_id::scope_resolution,
	operator_id::member_pointer_access,
	operator_id::index,
	operator_id::call,
	operator_id::increment,
	operator_id::decrement,
	operator_id::left_shift,
	operator_id::right_shift,
	operator_id::unknown,
	operator_id::ternary,
	operator_id::bitwise_or,
	operator_id::bitwise_xor,
	operator_id::bitwise_and,
	operator_id::less,
	operator_id::more,
	operator_id::plus,
	operator_id::minus,
	operator_id::times,
	operator_id::divide,
	operator_id::modulus,
	operator_id::member_access,
	operator_id::relational_not,
	operator_id::bitwise_inverse,
	operator_id::assignment,
	operator_id::unknown,
});

cscript::lexer::operator_token::operator_token(const index &index, const std::string &value, operator_id id)
	: token(index, value, static_cast<int>(token_id::operator_symbol), {}), id_(id){}

cscript::lexer::operator_token::operator_token(const index &index, std::string &&value, operator_id id)
	: token(index, static_cast<std::string &&>(value), static_cast<int>(token_id::operator_symbol), {}), id_(id){}

cscript::lexer::operator_id cscript::lexer::operator_token::get_id() const{
	return id_;
}
