#include "preprocessor_token.h"

cscript::lexer::preprocessor_token::include::include(const index &index, const std::string &value, int match_index, bool is_absolute)
	: token(index, value, match_index, {}), is_absolute_(is_absolute){}

cscript::lexer::preprocessor_token::include::include(const index &index, std::string &&value, int match_index, bool is_absolute)
	: token(index, value, match_index, {}), is_absolute_(is_absolute){}

bool cscript::lexer::preprocessor_token::include::is_absolute() const{
	return is_absolute_;
}
