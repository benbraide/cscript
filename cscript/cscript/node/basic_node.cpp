#include "basic_node.h"

cscript::node::basic::basic(id id, generic *parent)
	: id_(id), parent_(parent){}

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

bool cscript::node::basic::is(id id) const{
	return (id == id_);
}
