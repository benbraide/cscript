#include "type_cast_node.h"
#include "../common/env.h"

cscript::node::type_cast::type_cast(const lexer::token::index &index, ptr_type type, ptr_type value)
	: basic(id::type_cast, index), type_(type), value_(value){}

cscript::node::type_cast::~type_cast(){}

cscript::node::generic::ptr_type cscript::node::type_cast::clone(){
	return std::make_shared<type_cast>(index_, type_->clone(), value_->clone());
}

cscript::object::generic *cscript::node::type_cast::evaluate(){
	auto type = type_->get_type();
	if (type == nullptr)
		return common::env::error.set("", index_);

	auto value = value_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (value == nullptr)
		return common::env::error.set("", index_);

	auto is_ref = false, is_const = false;
	if (type_->is(id::type_with_storage)){
		auto attributes = type_->query<type_with_storage_class>()->get_attributes();
		is_ref = CSCRIPT_IS(attributes, memory::virtual_address::attribute::ref);
		is_const = CSCRIPT_IS(attributes, memory::virtual_address::attribute::constant);
	}

	if (is_ref && !is_const && value->is_constant())
		return common::env::error.set("", index_);

	auto cast_value = is_ref ? value->ref_cast(type.get()) : value->cast(type.get());
	if (cast_value == nullptr)
		return common::env::error.set("", index_);

	return cast_value;
}

cscript::node::generic::ptr_type cscript::node::type_cast::get_type() const{
	return type_;
}

cscript::node::generic::ptr_type cscript::node::type_cast::get_value() const{
	return value_;
}

std::string cscript::node::type_cast::print_() const{
	return ("type_cast<" + type_->print() + ">(" + value_->print() + ")");
}
