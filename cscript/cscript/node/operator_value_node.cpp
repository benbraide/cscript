#include "operator_value_node.h"

bool cscript::storage::operator<(const operator_key &left, const operator_key &right){
	return !(left == right);
}

bool cscript::storage::operator==(const operator_key &left, const operator_key &right){
	if (left.value_type != right.value_type)
		return false;

	if (left.type != nullptr || right.type != nullptr){
		if (left.type == nullptr || right.type == nullptr)
			return false;

		return left.type->is_same(right.type.get());
	}

	return (left.id == right.id && left.value == right.value);
}

cscript::node::operator_value::operator_value(const lexer::token::index &index, const key &key, const std::string &value)
	: basic(id::operator_value_, index), key_(key), value_(value){}

cscript::node::operator_value::~operator_value(){}

cscript::node::generic::ptr_type cscript::node::operator_value::clone(){
	return std::make_shared<operator_value>(index_, key_, value_);
}

cscript::object::generic *cscript::node::operator_value::evaluate(){
	return nullptr;
}

const cscript::node::operator_value::key &cscript::node::operator_value::get_key() const{
	return key_;
}

const std::string &cscript::node::operator_value::get_value() const{
	return value_;
}

std::string cscript::node::operator_value::print_() const{
	return ("operator" + value_);
}
