#include "function_definition.h"

#include "../common/env.h"
#include "../node/collection_node.h"
#include "../node/declaration_node.h"
#include "../object/ref_object.h"
#include "../storage/basic_storage.h"

cscript::function::definition::definition(const return_type_info &info, node_type parameters, node_type value)
	: info_(info), parameters_(parameters), value_(value){}

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

	bool is_void;
	if (common::env::error.has()){
		if (!common::env::error.is_return())
			return nullptr;

		is_void = (common::env::error.get() == nullptr);
	}
	else//No return
		is_void = true;

	if (!is_void){
		auto return_value = common::env::error.get();
		common::env::error.clear();

		if (info_.value->is_same(common::env::void_type.get()))
			return common::env::error.set("void function returning a value");

		object::generic::ptr_type object = CSCRIPT_IS(info_.attributes, memory::address_attribute::ref) ?
			std::make_shared<object::ref>(info_.value) : info_.value->create(info_.value);

		if (object == nullptr)
			return common::env::error.set("Return value validation failure");

		auto &memory_entry = object->get_memory();
		memory_entry.info.storage = &local_storage;
		if (CSCRIPT_IS(info_.attributes, memory::address_attribute::constant | memory::address_attribute::final_))
			CSCRIPT_SET(memory_entry.attributes, memory::address_attribute::constant);

		common::env::runtime.operand = { nullptr, return_value.get() };
		object->evaluate(object::generic::binary_info{ lexer::operator_id::assignment });
		memory_entry.info.storage = nullptr;

		if (!common::env::error.has())
			return common::env::temp_storage.add(object);
	}
	else if (!info_.value->is_same(common::env::void_type.get()))
		return common::env::error.set("Function must return a value");

	return nullptr;
}

cscript::function::definition::node_type cscript::function::definition::get_value() const{
	return value_;
}

cscript::function::definition::node_type cscript::function::definition::get_parameters() const{
	return parameters_;
}
