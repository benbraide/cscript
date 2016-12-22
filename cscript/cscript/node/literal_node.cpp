#include "literal_node.h"

cscript::node::literal::literal(const lexer::token::index &index, const std::string &value, const std::string &suffix, creator creator)
	: basic(id::literal), index_(index), value_(value), suffix_(suffix), creator_(creator){}

cscript::node::literal::~literal(){}

cscript::object::generic::ptr_type cscript::node::literal::evaluate(){
	try{
		return creator_(value_);
	}
	catch (const std::out_of_range &){
		common::env::error.set("Numeric value is too large for target type", index_);
	}
	catch (...){
		common::env::error.set("Could not interpret literal", index_);
	}

	return nullptr;
}
