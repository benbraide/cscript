#include "basic_node.h"

cscript::node::basic::basic(id id, const lexer::token::index &index, generic *parent)
	: id_(id), index_(index), parent_(parent){}

cscript::node::basic::~basic(){}

const cscript::node::generic *cscript::node::basic::get_parent() const{
	return parent_;
}

cscript::node::generic *cscript::node::basic::get_parent(){
	return parent_;
}

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
	auto tab = (parent_ == nullptr) ? 0 : parent_->get_print_tab();
	return (tab > 0) ? (std::string(tab, ' ') + print_()) : print_();
}

int cscript::node::basic::get_print_tab() const{
	return (parent_ == nullptr) ? 0 : parent_->get_print_tab();
}

std::string cscript::node::basic::print_() const{
	return "";
}
