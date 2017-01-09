#include "function_definition.h"

#include "../common/env.h"
#include "../node/collection_node.h"
#include "../node/declaration_node.h"
#include "../storage/basic_storage.h"

cscript::function::definition::definition(node_type parameters, node_type value)
	: parameters_(parameters), value_(value){}

cscript::function::definition::~definition(){}

cscript::object::generic *cscript::function::definition::call(storage::generic *storage){
	storage::basic local_storage(storage);
	auto previous_storage = common::env::runtime.storage;
	common::env::runtime.storage = &local_storage;

	auto &parameter_list = parameters_->query<node::collection>()->get_list();
	auto iter = parameter_list.begin();

	for (auto argument : common::env::runtime.arguments){
		auto declaration = (*iter)->query<node::declaration>();
		auto type = declaration->get_type();

		if (type->is(node::id::variadic_type)){

		}
		else{//Copy value
			auto object = declaration->allocate(type->is(node::id::auto_type) ? argument->get_type() : type->get_type());
			if (common::env::error.has())
				break;

			if (object == nullptr){
				common::env::error.set("");
				break;
			}

			common::env::runtime.operand = { nullptr, argument };
			object->evaluate(object::generic::binary_info{ lexer::operator_id::assignment });
			if (common::env::error.has())
				break;

			++iter;
		}
	}

	for (; iter != parameter_list.end(); ++iter){
		auto declaration = (*iter)->query<node::declaration>();
		if (declaration->get_type()->is(node::id::variadic_type)){
			break;
		}

		declaration->evaluate();
		if (common::env::error.has())
			break;
	}

	if (!common::env::error.has()){
		for (auto statement : value_->query<node::collection>()->get_list()){
			statement->evaluate();
			common::env::temp_storage.clear();
			if (common::env::error.has())
				break;
		}
	}

	common::env::runtime.storage = previous_storage;
	common::env::runtime.arguments.clear();

	return nullptr;
}

cscript::function::definition::node_type cscript::function::definition::get_value() const{
	return value_;
}

cscript::function::definition::node_type cscript::function::definition::get_parameters() const{
	return parameters_;
}
