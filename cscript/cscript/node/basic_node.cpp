#include "basic_node.h"

cscript::node::basic::basic(id id, const lexer::token::index &index)
	: id_(id), index_(index){}

cscript::node::basic::~basic(){}

cscript::node::id cscript::node::basic::get_id() const{
	return id_;
}

const cscript::lexer::token::index &cscript::node::basic::get_index() const{
	return index_;
}

bool cscript::node::basic::is(id id) const{
	return (id == id_);
}

std::string cscript::node::basic::print() const{
	return print_();
}

std::string cscript::node::basic::print_() const{
	return "";
}
