#include "string_source.h"

cscript::lexer::source::string::string(const std::string &buffer)
	: buffer_(buffer){
	current_ = buffer_.begin();
	end_ = buffer_.end();
}

cscript::lexer::source::string::string(std::string &&buffer)
	: buffer_(static_cast<std::string &&>(buffer)){
	current_ = buffer_.begin();
	end_ = buffer_.end();
}
