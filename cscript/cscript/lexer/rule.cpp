#include "rule.h"

#define CSCRIPT_LEXER_RULE_REAL			"[0-9]+\\.[0-9]*"
#define CSCRIPT_LEXER_RULE_REAL2		"\\.[0-9]+"
#define CSCRIPT_LEXER_RULE_EXCLUDES		"\\s\\r\\n\\w\\d\\(\\)\\[\\]\\{\\}\"'`\\/\\\\,;$_"
#define CSCRIPT_LEXER_RULE_ID			"[$_a-zA-Z][$_a-zA-Z0-9]*"

cscript::lexer::rule::rule(){
	std::vector<std::string> reserved({
		"class", "enum", "namespace", "using", "new", "delete", "typedef", "operator", "sizeof", "typeof", "echo", "true", "false",
		"auto", "any", "void", "bool", "bit", "byte", "nan", "char", "wchar", "short", "int", "long", "unsigned", "float", "double",
		"string", "array", "try", "catch", "finally", "throw", "if", "else", "unless", "while", "for", "until", "do", "switch", "case",
		"default", "break", "continue", "return", "decltype", "function", "nullptr", "pointer_t", "thread_local", "static", "const",
		"final", "ref", "private", "protected", "public", "type_cast", "self", "__placeholder"
	});

	std::vector<std::string> lang({
		CSCRIPT_LEXER_RULE_REAL "[eE][+-]?[0-9]+",					//Exponentiated number
		CSCRIPT_LEXER_RULE_REAL2 "[eE][+-]?[0-9]+",					//Exponentiated number
		"[0-9]+[eE][+-]?[0-9]+",									//Exponentiated number
		CSCRIPT_LEXER_RULE_REAL,									//Real number
		CSCRIPT_LEXER_RULE_REAL2,									//Real number
		"[2][0-6]r[0-9a-zA-Z]+",									//Radix integer
		"[2-9]r[0-9a-zA-Z]+",										//Radix integer
		"[1][0-9]r[0-9a-zA-Z]+",									//Radix integer
		"0x[0-9a-fA-F]+",											//Hexadecimal integer
		"[1-9][0-9]*",												//Decimal integer
		"[0][0-7]*",												//Octal integer
		"0b[01]+",													//Binary integer
		CSCRIPT_LEXER_RULE_ID,										//Identifier
		"\"",														//Escaped double quoted string
		"'",														//Escaped single quoted string
		"`",														//Escaped back quoted string
		"@\"",														//Double quoted string
		"@'",														//Single quoted string
		"@`",														//Back quoted string
		"@" CSCRIPT_LEXER_RULE_ID,									//Property
		"@\\[",														//Property list
		"#include\\b",												//Preprocessor
		"#define\\b",												//Preprocessor
		"#undef\\b",												//Preprocessor
		"#ifdef\\b",												//Preprocessor
		"#ifndef\\b",												//Preprocessor
		"#else\\b",													//Preprocessor
		"#endif\\b",												//Preprocessor
		"#pragma\\b",												//Preprocessor
		"\\[",														//Symbol
		"\\(",														//Symbol
		"\\{",														//Symbol
		"\\]",														//Symbol
		"\\)",														//Symbol
		"\\}",														//Symbol
		";",														//Symbol
		",",														//Symbol
		"\\\\",														//Symbol
		"\\/\\/",													//Single line comment
		"\\/\\*",													//Multiline comment
		"\\r\\n",													//New line
		"\\n\\r",													//New line
		"[\\r\\n]",													//New line
		"[\\s]+",													//Blanks
		"[^" CSCRIPT_LEXER_RULE_EXCLUDES "]+",						//Symbol
		"\\/.*",													//Symbol
	});

	std::string combined_value;
	for (auto &value : reserved){
		if (combined_value.empty())
			combined_value = "(" + value + "\\b)";
		else//Append
			combined_value += "|(" + value + "\\b)";
	}

	for (auto &value : lang){
		if (combined_value.empty())
			combined_value = "(" + value + ")";
		else//Append
			combined_value += "|(" + value + ")";
	}

	compiled_.assign("^" + combined_value, boost::regex_constants::optimize);
}

bool cscript::lexer::rule::match(const std::string &value, match_info &matched) const{
	return match(value.begin(), value.end(), matched);
}

bool cscript::lexer::rule::match(iterator_type start, iterator_type end, match_info &matched) const{
	boost::match_results<iterator_type> results;
	if (!boost::regex_search(start, end, results, compiled_, boost::regex_constants::match_single_line))
		return false;

	matched.index = get_match_index_(results);
	matched.size = results.length();

	return true;
}

bool cscript::lexer::rule::match(const char *start, const char *end, match_info &matched) const{
	boost::match_results<const char *> results;
	if (!boost::regex_search(start, end, results, compiled_))
		return false;

	matched.index = get_match_index_(results);
	matched.size = results.length();

	return true;
}

int cscript::lexer::rule::get_error_index() const{
	return -1;
}

cscript::lexer::token_id cscript::lexer::rule::map_index(int index) const{
	if (index < 0)//Error index
		return token_id::error;

	return (static_cast<size_type>(index) < map_.size()) ? map_[index] : static_cast<token_id>(index);
}

const cscript::lexer::generic_rule::regex_type &cscript::lexer::rule::get_compiled() const{
	return compiled_;
}

const cscript::lexer::rule::list_type cscript::lexer::rule::map_({
	token_id::class_,
	token_id::enum_,
	token_id::namespace_,
	token_id::using_,
	token_id::new_,
	token_id::delete_,
	token_id::typedef_,
	token_id::operator_,
	token_id::sizeof_,
	token_id::typeof,
	token_id::echo,
	token_id::true_,
	token_id::false_,
	token_id::auto_,
	token_id::any,
	token_id::void_,
	token_id::bool_,
	token_id::bit,
	token_id::byte,
	token_id::nan,
	token_id::char_,
	token_id::wchar,
	token_id::short_,
	token_id::int_,
	token_id::long_,
	token_id::unsigned_,
	token_id::float_,
	token_id::double_,
	token_id::string,
	token_id::array_,
	token_id::try_,
	token_id::catch_,
	token_id::finally_,
	token_id::throw_,
	token_id::if_,
	token_id::else_,
	token_id::unless,
	token_id::while_,
	token_id::for_,
	token_id::until,
	token_id::do_,
	token_id::switch_,
	token_id::case_,
	token_id::default_,
	token_id::break_,
	token_id::continue_,
	token_id::return_,
	token_id::decltype_,
	token_id::function,
	token_id::nullptr_,
	token_id::pointer,
	token_id::tls,
	token_id::static_,
	token_id::const_,
	token_id::final_,
	token_id::ref,
	token_id::private_,
	token_id::protected_,
	token_id::public_,
	token_id::type_cast,
	token_id::self,
	token_id::placeholder,
	token_id::exp,
	token_id::exp,
	token_id::exp,
	token_id::real,
	token_id::real,
	token_id::rad,
	token_id::rad,
	token_id::rad,
	token_id::hex,
	token_id::dec,
	token_id::oct,
	token_id::bin,
	token_id::identifier,
	token_id::esc_quote_dbl,
	token_id::esc_quote_sng,
	token_id::esc_quote_back,
	token_id::quote_dbl,
	token_id::quote_sng,
	token_id::quote_back,
	token_id::prop_sng,
	token_id::prop_multi,
	token_id::prep_incl,
	token_id::prep_def,
	token_id::prep_undef,
	token_id::prep_ifdef,
	token_id::prep_ifndef,
	token_id::prep_else,
	token_id::prep_endif,
	token_id::prep_pragma,
	token_id::open_sq,
	token_id::open_par,
	token_id::open_cur,
	token_id::close_sq,
	token_id::close_par,
	token_id::close_cur,
	token_id::semi_colon,
	token_id::comma,
	token_id::backslash,
	token_id::comment_sng,
	token_id::comment_mult,
	token_id::new_line,
	token_id::new_line,
	token_id::new_line,
	token_id::blank,
	token_id::symbol,
	token_id::symbol,
});
