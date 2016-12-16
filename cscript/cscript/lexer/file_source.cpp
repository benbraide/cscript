#include "file_source.h"

cscript::lexer::source::file::file(const std::string &path)
	: buffer_(path, boost::iostreams::mapped_file::readonly){
	if (buffer_.is_open()){
		current_ = buffer_.const_data();
		end_ = current_ + buffer_.size();
	}
}

bool cscript::lexer::source::file::is_open() const{
	return buffer_.is_open();
}
