#include "collection_node.h"
#include "../common/env.h"

cscript::node::collection::collection(const lexer::token::index &index)
	: basic(id::collection, index){}

cscript::node::collection::~collection(){}

cscript::node::generic::ptr_type cscript::node::collection::clone(){
	auto value = std::make_shared<collection>(index_);
	for (auto entry : list_)
		value->append(entry->clone());

	return value;
}

cscript::object::generic *cscript::node::collection::evaluate(){
	object::generic *value;
	for (auto entry : list_){
		value = entry->evaluate();
		if (common::env::error.has())
			break;
	}

	return value;
}

cscript::node::collection &cscript::node::collection::append(ptr_type value){
	list_.push_back(value);
	return *this;
}

cscript::node::collection::list_type &cscript::node::collection::get_list(){
	return list_;
}
