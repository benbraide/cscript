#include "statement_node.h"

cscript::node::statement::statement(const lexer::token::index &index, ptr_type collection)
	: basic(id::statement, index), collection_(collection){}

cscript::node::statement::~statement(){}

cscript::node::generic::ptr_type cscript::node::statement::clone(){
	return std::make_shared<statement>(index_, collection_->clone());
}

cscript::object::generic *cscript::node::statement::evaluate(){
	collection_->evaluate();
	return nullptr;
}

cscript::node::generic::ptr_type cscript::node::statement::get_collection(){
	return collection_;
}
