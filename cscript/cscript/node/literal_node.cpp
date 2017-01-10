#include "literal_node.h"
#include "../common/env.h"

cscript::node::literal::literal(const lexer::token::index &index, lexer::token_id id, const std::string &value, creator creator)
	: basic(id::literal, index), id_(id), value_(value), creator_(creator){
	try{
		info_.value = creator(value_);
		if (common::env::error.has()){
			info_.error = common::env::error.get()->to_string();
			common::env::error.clear();
		}
	}
	catch (const std::out_of_range &){
		info_.error = "Numeric value is too large for target type";
	}
	catch (...){
		info_.error = "Could not interpret literal";
	}
}

cscript::node::literal::~literal(){}

cscript::node::generic::ptr_type cscript::node::literal::clone(){
	return std::make_shared<literal>(index_, id_, value_, creator_);
}

cscript::object::generic *cscript::node::literal::evaluate(){
	return (info_.value == nullptr) ? common::env::error.set(info_.error, index_) : info_.value.get();
}

cscript::lexer::token_id cscript::node::literal::get_token_id() const{
	return id_;
}

std::string cscript::node::literal::print_() const{
	return value_;
}
