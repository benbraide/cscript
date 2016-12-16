#include "token.h"

cscript::lexer::token::token(){}

cscript::lexer::token::token(const index &index, const std::string &value, int match_index)
	: index_(index), value_(value), match_index_(match_index){}

cscript::lexer::token::token(const index &index, std::string &&value, int match_index)
	: index_(index), value_(static_cast<std::string &&>(value)), match_index_(match_index){}

cscript::lexer::token::~token(){}

bool cscript::lexer::token::is_processed() const{
	return false;
}

cscript::lexer::token &cscript::lexer::token::update(const index &index, const std::string &value, int match_index){
	index_ = index;
	value_ = value;
	match_index = match_index_;
	return *this;
}

cscript::lexer::token &cscript::lexer::token::update(const index &index, std::string &&value, int match_index){
	index_ = index;
	value_ = static_cast<std::string &&>(value);
	match_index = match_index_;
	return *this;
}

cscript::lexer::token &cscript::lexer::token::update(const index &index){
	index_ = index;
	return *this;
}

cscript::lexer::token &cscript::lexer::token::update(const std::string &value){
	value_ = value;
	return *this;
}

cscript::lexer::token &cscript::lexer::token::update(std::string &&value){
	value_ = static_cast<std::string &&>(value);
	return *this;
}

cscript::lexer::token &cscript::lexer::token::update(int match_index){
	match_index = match_index_;
	return *this;
}

const cscript::lexer::token::index& cscript::lexer::token::get_index() const{
	return index_;
}

const std::string &cscript::lexer::token::get_value() const{
	return value_;
}

int cscript::lexer::token::get_line() const{
	return index_.line;
}

int cscript::lexer::token::get_column() const{
	return index_.column;
}

int cscript::lexer::token::get_match_index() const{
	return match_index_;
}

std::string cscript::lexer::token::to_string() const{
	return (value_ + " -> " + index_to_string(index_));
}

std::string cscript::lexer::token::index_to_string(const index &value){
	return ("line " + std::to_string(value.line) + ", col " + std::to_string(value.column));
}

bool cscript::lexer::preprocessed_token::is_processed() const{
	return true;
}

const std::type_info &cscript::lexer::preprocessed_token::get_type_info() const{
	return value_type_info_;
}

const boost::any &cscript::lexer::preprocessed_token::get_processed_value() const{
	return processed_value_;
}

cscript::lexer::error_token::error_token(const index &index, const std::string &value)
	: token(index, value, -1){}

cscript::lexer::error_token::error_token(const index &index, std::string &&value)
	: token(index, static_cast<std::string &&>(value), -1){}
