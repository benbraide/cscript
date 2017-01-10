#include "function_node.h"
#include "declaration_node.h"
#include "collection_node.h"

#include "../common/env.h"

#include "../type/primitive_type.h"
#include "../object/function_object.h"

cscript::node::function::function(const lexer::token::index &index, ptr_type declaration, ptr_type parameters, ptr_type definition)
	: basic(id::function, index), declaration_(declaration), parameters_(parameters), definition_(definition){}

cscript::node::function::~function(){}

cscript::node::generic::ptr_type cscript::node::function::clone(){
	if (definition_ == nullptr)
		return std::make_shared<function>(index_, declaration_->clone(), parameters_->clone(), nullptr);

	return std::make_shared<function>(index_, declaration_->clone(), parameters_->clone(), definition_->clone());
}

bool cscript::node::function::is(id id) const{
	return (id == id_ || (id == node::id::block && definition_ != nullptr));
}

cscript::object::generic *cscript::node::function::evaluate(){
	resolve_type_();
	if (common::env::error.has())
		return nullptr;

	auto declaration_node = declaration_->query<declaration>();
	auto key = declaration_node->get_identifier()->get_key();

	auto entry = common::env::runtime.storage->find(key);
	if (entry == nullptr){//New entry
		if (key.empty())
			return common::env::error.set("Invalid key", index_);

		auto base = std::make_shared<cscript::function::basic>(key, common::env::runtime.storage);
		common::env::runtime.storage->store_function(base);

		auto function_object = std::make_shared<object::primitive::function_object>(
			reinterpret_cast<object::primitive::function_object::value_type>(base.get()));

		function_object->get_memory().info.storage = common::env::runtime.storage;
		common::env::runtime.storage->add(key).set(function_object);

		return insert_(base.get());
	}
	
	auto entry_object = entry->get_object();
	auto function_object = (entry_object == nullptr) ? nullptr : entry_object->query<object::primitive::function_object>();
	if (function_object == nullptr)
		return common::env::error.set("'" + key + "' name already exists", index_);

	return insert_(function_object->get_function_value());
}

cscript::node::generic::ptr_type cscript::node::function::get_declaration() const{
	return declaration_;
}

cscript::node::generic::ptr_type cscript::node::function::get_parameters() const{
	return parameters_;
}

cscript::node::generic::ptr_type cscript::node::function::get_definition() const{
	return definition_;
}

std::string cscript::node::function::print_() const{
	if (definition_ == nullptr)
		return (declaration_->print() + "(" + parameters_->print() + ")");

	return (declaration_->print() + "(" + parameters_->print() + "){...}");
}

std::nullptr_t cscript::node::function::insert_(cscript::function::generic *value){
	auto matched = value->get_matched(type_value_.get());
	if (matched == nullptr){//Add entry
		auto entry = std::make_shared<cscript::function::entry>(value->get_name(),
			common::env::runtime.storage, type_value_, parameter_limits_);

		common::env::runtime.storage->store_function(entry);
		value->add(*(matched = entry.get()));
	}
	else if (definition_ == nullptr)//Multiple declarations
		return common::env::error.set("Function has already been declared", index_);

	if (definition_ != nullptr){
		if (matched->get_definition() != nullptr)//Multiple definitions
			return common::env::error.set("Function has already been defined", index_);

		auto return_type = declaration_->query<declaration>()->get_type();
		auto with_storage = return_type->query<type_with_storage_class>();

		cscript::function::definition::return_type_info info{
			return_type->get_type(),
			(with_storage == nullptr) ? memory::address_attribute::nil : with_storage->get_attributes()
		};

		if (info.value == nullptr)
			return common::env::error.set("Could not resolve return type", index_);

		dynamic_cast<cscript::function::entry *>(matched)->set_definition(
			std::make_shared<cscript::function::definition>(info, parameters_, definition_));
	}

	return nullptr;
}

std::nullptr_t cscript::node::function::resolve_type_(){
	if (type_value_ != nullptr)
		return nullptr;

	parameter_limits_ = {};
	auto return_type = declaration_->query<declaration>()->get_type_value();
	if (return_type == nullptr)
		return common::env::error.set("", index_);

	type::function::list_type parameter_types;
	for (auto parameter : parameters_->query<collection>()->get_list()){
		auto parameter_type = parameter->query<declaration>()->get_type_value();
		if (parameter_type == nullptr)
			return common::env::error.set("", index_);

		parameter_types.push_back(parameter_type);
		if (!parameter_type->is_variadic()){
			if (parameter->query<initialization_declaration>() == nullptr)
				++parameter_limits_.minimum;
			++parameter_limits_.maximum;
		}
		else//Infinite
			parameter_limits_.maximum = -1;
	}

	type_value_ = std::make_shared<type::function>(return_type, parameter_types);
	return nullptr;
}
