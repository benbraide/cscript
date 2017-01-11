#include "type_node.h"
#include "collection_node.h"

#include "../common/env.h"
#include "../object/type_object.h"

cscript::node::primitive_type::primitive_type(const lexer::token::index &index, lexer::token_id value)
	: basic(id::type, index), value_(value){}

cscript::node::primitive_type::~primitive_type(){}

cscript::node::generic::ptr_type cscript::node::primitive_type::clone(){
	return std::make_shared<primitive_type>(index_, value_);
}

bool cscript::node::primitive_type::is(id id) const{
	if (id == node::id::type || id == node::id::type_compatible)
		return true;

	if (id == node::id::void_type)
		return (value_ == lexer::token_id::void_);

	if (id == node::id::pointer_type)
		return (value_ == lexer::token_id::pointer);

	if (id == node::id::array_type)
		return (value_ == lexer::token_id::array_);

	if (id == node::id::function_type)
		return (value_ == lexer::token_id::function);

	return (id == node::id::auto_type) ? (value_ == lexer::token_id::auto_) : false;
}

cscript::object::generic *cscript::node::primitive_type::evaluate(){
	auto type_value = get_type();
	if (type_value == nullptr)
		return common::env::error.set("'" + print_() + "' type not found", index_);

	return common::env::temp_storage.add(std::make_shared<object::primitive::type_object>(type_value));
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
		return common::env::string_type;
	case lexer::token_id::pointer:
		return common::env::pointer_type;
	case lexer::token_id::array_:
		return common::env::array_type;
	case lexer::token_id::function:
		return common::env::function_type;
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
	case lexer::token_id::pointer:
		return "pointer_t";
	case lexer::token_id::array_:
		return "array";
	case lexer::token_id::function:
		return "function";
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

	return (id == node::id::void_type || id == node::id::auto_type || id == node::id::array_type ||
		id == node::id::pointer_type || id == node::id::function_type || id == node::id::placeholder) ? type_->is(id) : false;
}

cscript::object::generic *cscript::node::type_with_storage_class::evaluate(){
	return common::env::error.set("Cannot evaluate expression", index_);
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

cscript::node::pointer_type::pointer_type(const lexer::token::index &index, ptr_type base_type)
	: basic(id::pointer_type, index), base_type_(base_type){}

cscript::node::pointer_type::~pointer_type(){}

cscript::node::generic::ptr_type cscript::node::pointer_type::clone(){
	return std::make_shared<pointer_type>(index_, base_type_->clone());
}

bool cscript::node::pointer_type::is(id id) const{
	return (id == id_ || id == node::id::type_compatible);
}

cscript::object::generic *cscript::node::pointer_type::evaluate(){
	auto type_value = get_type();
	if (type_value == nullptr)
		return common::env::error.set("'" + base_type_->print() + "' type not found", index_);

	return common::env::temp_storage.add(std::make_shared<object::primitive::type_object>(type_value));
}

cscript::type::generic::ptr_type cscript::node::pointer_type::get_type(){
	if (type_value_ == nullptr){
		auto base_type_value = base_type_->get_type();
		if (common::env::error.has() || base_type_value == nullptr)
			return nullptr;

		type_value_ = std::make_shared<type::pointer>(base_type_value);
	}

	return type_value_;
}

cscript::node::generic::ptr_type cscript::node::pointer_type::get_base_type() const{
	return base_type_;
}

std::string cscript::node::pointer_type::print_() const{
	return ("pointer_t<" + base_type_->print() + ">");
}

cscript::node::array_type::array_type(const lexer::token::index &index, ptr_type base_type)
	: basic(id::array_type, index), base_type_(base_type){}

cscript::node::array_type::~array_type(){}

cscript::node::generic::ptr_type cscript::node::array_type::clone(){
	return std::make_shared<array_type>(index_, base_type_->clone());
}

bool cscript::node::array_type::is(id id) const{
	return (id == id_ || id == node::id::type_compatible);
}

cscript::object::generic *cscript::node::array_type::evaluate(){
	return nullptr;
}

cscript::type::generic::ptr_type cscript::node::array_type::get_type(){
	if (type_value_ == nullptr){
		auto base_type_value = base_type_->get_type();
		if (common::env::error.has() || base_type_value == nullptr)
			return nullptr;

		type_value_ = std::make_shared<type::array>(base_type_value);
	}

	return type_value_;
}

cscript::node::generic::ptr_type cscript::node::array_type::get_base_type() const{
	return base_type_;
}

std::string cscript::node::array_type::print_() const{
	return ("array<" + base_type_->print() + ">");
}

cscript::node::function_type::function_type(const lexer::token::index &index, ptr_type return_type, ptr_type parameter_types)
	: basic(id::function_type, index), return_type_(return_type), parameter_types_(parameter_types){}

cscript::node::function_type::~function_type(){}

cscript::node::generic::ptr_type cscript::node::function_type::clone(){
	return std::make_shared<function_type>(index_, return_type_->clone(), parameter_types_->clone());
}

bool cscript::node::function_type::is(id id) const{
	return (id == id_ || id == node::id::type_compatible);
}

cscript::object::generic *cscript::node::function_type::evaluate(){
	auto type_value = get_type();
	if (type_value == nullptr)
		return common::env::error.set("Failed to resolve function type", index_);

	return common::env::temp_storage.add(std::make_shared<object::primitive::type_object>(type_value));
}

cscript::type::generic::ptr_type cscript::node::function_type::get_type(){
	if (type_value_ == nullptr){
		auto return_type_value = return_type_->get_type();
		if (common::env::error.has() || return_type_value == nullptr)
			return nullptr;

		type::function::list_type parameter_type_values;
		for (auto type_node : parameter_types_->query<collection>()->get_list()){
			auto type_value = type_node->get_type();
			if (common::env::error.has() || type_value == nullptr)
				return nullptr;

			parameter_type_values.push_back(type_value);
		}

		type_value_ = std::make_shared<type::function>(return_type_value, parameter_type_values);
	}

	return type_value_;
}

cscript::node::generic::ptr_type cscript::node::function_type::get_return_type() const{
	return return_type_;
}

cscript::node::generic::ptr_type cscript::node::function_type::get_parameter_types() const{
	return parameter_types_;
}

std::string cscript::node::function_type::print_() const{
	return ("function<" + return_type_->print() + "(" + parameter_types_->print() + ")>");
}
