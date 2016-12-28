#include "preprocessor_token.h"

cscript::lexer::preprocessor_token::include::include(const index &index, const std::string &value, int match_index, bool is_absolute)
	: token(index, value, match_index, {}), is_absolute_(is_absolute){}

cscript::lexer::preprocessor_token::include::include(const index &index, std::string &&value, int match_index, bool is_absolute)
	: token(index, value, match_index, {}), is_absolute_(is_absolute){}

bool cscript::lexer::preprocessor_token::include::is_absolute() const{
	return is_absolute_;
}

cscript::lexer::preprocessor_token::define::define(const index &index, const std::string &value, int match_index, const list_type &list)
	: token(index, value, match_index, {}), list_(list){}

cscript::lexer::preprocessor_token::define::define(const index &index, const std::string &value, int match_index, list_type &&list)
	: token(index, value, match_index, {}), list_(static_cast<list_type &&>(list)){}

const cscript::lexer::preprocessor_token::define::list_type &cscript::lexer::preprocessor_token::define::get_list() const{
	return list_;
}

cscript::lexer::preprocessor_token::define::list_type &cscript::lexer::preprocessor_token::define::get_list(){
	return list_;
}

cscript::lexer::preprocessor_token::undefine::undefine(const index &index, const std::string &value, int match_index)
	: token(index, value, match_index, {}){}

cscript::lexer::preprocessor_token::conditional::conditional(const index &index, const std::string &value, int match_index, state states)
	: token(index, value, match_index, {}), states_(states){}

bool cscript::lexer::preprocessor_token::conditional::is_rejected() const{
	return CSCRIPT_IS(states_, state::is_rejected);
}

bool cscript::lexer::preprocessor_token::conditional::is_else() const{
	return CSCRIPT_IS(states_, state::is_else);
}
