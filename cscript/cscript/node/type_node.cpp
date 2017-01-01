#include "type_node.h"
#include "../common/env.h"

cscript::node::primitive_type::primitive_type(const lexer::token::index &index, lexer::token_id value)
	: basic(id::type, index), value_(value){}

cscript::node::primitive_type::~primitive_type(){}

cscript::node::generic::ptr_type cscript::node::primitive_type::clone(){
	return std::make_shared<primitive_type>(index_, value_);
}

bool cscript::node::primitive_type::is(id id) const{
	if (id == node::id::type || id == node::id::type_compatible)
		return true;

	return (id == node::id::auto_type) ? (value_ == lexer::token_id::auto_) : false;
}

cscript::object::generic *cscript::node::primitive_type::evaluate(){
	return nullptr;
}

std::string cscript::node::primitive_type::get_key(){
	return print_();
}

cscript::storage::generic *cscript::node::primitive_type::get_storage(){
	auto type = get_type();
	return (type == nullptr) ? nullptr : type->query<storage::generic>();
}

cscript::type::generic::ptr_type cscript::node::primitive_type::get_type(){
	switch (value_){
	case lexer::token_id::auto_:
		return common::env::auto_type;
	case lexer::token_id::any:
		return common::env::any_type;
	case lexer::token_id::void_:
		return common::env::void_type;
	case lexer::token_id::bool_:
		return common::env::bool_type;
	case lexer::token_id::bit:
		return common::env::bit_type;
	case lexer::token_id::byte:
		return common::env::byte_type;
	case lexer::token_id::char_:
		return common::env::char_type;
	case lexer::token_id::uchar:
		return common::env::uchar_type;
	case lexer::token_id::wchar:
		return common::env::wchar_type;
	case lexer::token_id::short_:
		return common::env::short_type;
	case lexer::token_id::ushort:
		return common::env::ushort_type;
	case lexer::token_id::int_:
		return common::env::int_type;
	case lexer::token_id::uint:
		return common::env::uint_type;
	case lexer::token_id::long_:
		return common::env::long_type;
	case lexer::token_id::ulong:
		return common::env::ulong_type;
	case lexer::token_id::llong:
		return common::env::llong_type;
	case lexer::token_id::ullong:
		return common::env::ullong_type;
	case lexer::token_id::float_:
		return common::env::float_type;
	case lexer::token_id::double_:
		return common::env::double_type;
	case lexer::token_id::ldouble:
		return common::env::ldouble_type;
	case lexer::token_id::string:
		//return common::env::string_type;
	default:
		break;
	}

	return nullptr;
}

cscript::lexer::token_id cscript::node::primitive_type::get_value() const{
	return value_;
}

std::string cscript::node::primitive_type::print_() const{
	switch (value_){
	case lexer::token_id::auto_:
		return "auto";
	case lexer::token_id::any:
		return "any";
	case lexer::token_id::void_:
		return "void";
	case lexer::token_id::bool_:
		return "bool";
	case lexer::token_id::bit:
		return "bit";
	case lexer::token_id::byte:
		return "byte";
	case lexer::token_id::char_:
		return "char";
	case lexer::token_id::uchar:
		return "unsigned char";
	case lexer::token_id::wchar:
		return "wchar";
	case lexer::token_id::short_:
		return "short";
	case lexer::token_id::ushort:
		return "unsigned short";
	case lexer::token_id::int_:
		return "int";
	case lexer::token_id::uint:
		return "unsigned int";
	case lexer::token_id::long_:
		return "long";
	case lexer::token_id::ulong:
		return "unsigned long";
	case lexer::token_id::llong:
		return "long long";
	case lexer::token_id::ullong:
		return "unsigned long long";
	case lexer::token_id::float_:
		return "float";
	case lexer::token_id::double_:
		return "double";
	case lexer::token_id::ldouble:
		return "long double";
	case lexer::token_id::string:
		return "string";
	default:
		break;
	}

	return "";
}

cscript::node::type_with_storage_class::type_with_storage_class(const lexer::token::index &index,
	attribute_type attributes, ptr_type type, const std::string &value)
	: basic(id::type_with_storage, index), attributes_(attributes), type_(type), value_(value){}

cscript::node::type_with_storage_class::~type_with_storage_class(){}

cscript::node::generic::ptr_type cscript::node::type_with_storage_class::clone(){
	return std::make_shared<type_with_storage_class>(index_, attributes_, type_->clone(), value_);
}

bool cscript::node::type_with_storage_class::is(id id) const{
	if (id == node::id::type_with_storage || id == node::id::type_compatible)
		return true;

	return (id == node::id::auto_type || id == node::id::array_type || id == node::id::pointer_type ||
		id == node::id::function_type || id == node::id::placeholder) ? type_->is(id) : false;
}

cscript::object::generic *cscript::node::type_with_storage_class::evaluate(){
	return common::env::error.set("", index_);
}

std::string cscript::node::type_with_storage_class::get_key(){
	return type_->get_key();
}

cscript::storage::generic *cscript::node::type_with_storage_class::get_storage(){
	return type_->get_storage();
}

cscript::type::generic::ptr_type cscript::node::type_with_storage_class::get_type(){
	return type_->get_type();
}

cscript::node::type_with_storage_class::attribute_type cscript::node::type_with_storage_class::get_attributes() const{
	return attributes_;
}

cscript::node::generic::ptr_type cscript::node::type_with_storage_class::get_value() const{
	return type_;
}

std::string cscript::node::type_with_storage_class::print_() const{
	return (value_ + type_->print());
}
