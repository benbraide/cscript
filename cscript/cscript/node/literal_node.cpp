#include "literal_node.h"

cscript::node::literal::literal(const lexer::token::index &index, const std::string &value, const std::string &suffix, creator creator, generic *parent)
	: basic(id::literal, index, parent), value_(value), suffix_(suffix){
	try{
		info_.value = creator(value_);
	}
	catch (const std::out_of_range &){
		info_.error = "Numeric value is too large for target type";
	}
	catch (...){
		info_.error = "Could not interpret literal";
	}
}

cscript::node::literal::~literal(){}

cscript::object::generic::ptr_type cscript::node::literal::evaluate(){
	return (info_.value == nullptr) ? common::env::error.set(info_.error, index_) : info_.value;
}

std::string cscript::node::literal::print_() const{
	return (value_ + suffix_);
}
