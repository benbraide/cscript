#include "scanner.h"

cscript::lexer::scanner::scanner(generic_source &source)
	: source_(source){}

cscript::lexer::generic_source &cscript::lexer::scanner::branch(){
	return source_.branch();
}

cscript::lexer::generic_source &cscript::lexer::scanner::unbranch(){
	return source_.unbranch();
}

cscript::lexer::generic_source &cscript::lexer::scanner::enable_multi_thread(bool enabled){
	return source_.enable_multi_thread(enabled);
}

cscript::lexer::generic_source &cscript::lexer::scanner::disable_multi_thread(){
	return source_.disable_multi_thread();
}

cscript::lexer::generic_source &cscript::lexer::scanner::advance_marker(int offset){
	return source_.advance_marker(offset);
}

cscript::lexer::generic_source::token_type cscript::lexer::scanner::next(source_info &info, int count){
	return source_.next(info, count);
}

cscript::lexer::generic_source::token_type cscript::lexer::scanner::peek(source_info &info, int count){
	return source_.peek(info, count);
}

cscript::lexer::generic_source &cscript::lexer::scanner::ignore(source_info &info, int count){
	return source_.ignore(info, count);
}

cscript::lexer::generic_source &cscript::lexer::scanner::cache(source_info &info, int count){
	return source_.cache(info, count);
}

char cscript::lexer::scanner::get_char(int count){
	return source_.get_char(count);
}

char cscript::lexer::scanner::peek_char(int count) const{
	return source_.peek_char(count);
}

const cscript::lexer::token::index &cscript::lexer::scanner::get_index() const{
	return source_.get_index();
}

bool cscript::lexer::scanner::has_more() const{
	return source_.has_more();
}

bool cscript::lexer::scanner::is_branched() const{
	return source_.is_branched();
}

bool cscript::lexer::scanner::is_multi_threaded() const{
	return source_.is_multi_threaded();
}

int cscript::lexer::scanner::get_cached_size() const{
	return source_.get_cached_size();
}

cscript::lexer::generic_scanner &cscript::lexer::scanner::save(token_type value){
	return *this;
}
