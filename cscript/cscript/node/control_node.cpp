#include "control_node.h"
#include "collection_node.h"

#include "../common/env.h"

cscript::node::control::control(id id, const lexer::token::index &index, option options)
	: basic(id, index), options_(options){}

cscript::node::control::~control(){}

bool cscript::node::control::is(id id) const{
	return (id == id_ || (id == node::id::block && !CSCRIPT_IS(options_, option::value)));
}

cscript::object::generic *cscript::node::control::evaluate(){
	auto previous_storage = common::env::runtime.storage;

	storage::basic local_storage(common::env::runtime.storage);
	common::env::runtime.storage = &local_storage;

	init_();
	if (common::env::error.has()){
		common::env::runtime.storage = previous_storage;
		return nullptr;
	}

	auto truth = CSCRIPT_IS(options_, option::skip_first) ? true : truth_();
	if (common::env::error.has()){
		common::env::runtime.storage = previous_storage;
		return nullptr;
	}

	auto body = get_body_();
	auto statements = (body == nullptr) ? nullptr : body->query<collection>();

	auto iterate = CSCRIPT_IS(options_, option::iterate);
	object::generic *value = nullptr;

	if (truth){
		do{
			if (body != nullptr){
				storage::basic block_storage(&local_storage);
				block_storage.use(local_storage);

				if (statements != nullptr){
					for (auto statement : statements->get_list()){
						common::env::runtime = { &block_storage };
						value = statement->evaluate();

						common::env::temp_storage.clear();
						if (common::env::error.has())
							break;
					}
				}
				else{//Single
					common::env::runtime = {};
					value = body->evaluate();
				}

				common::env::runtime.storage = &local_storage;
			}
			
			if (common::env::error.has()){
				if (!iterate)
					break;

				if (!common::env::error.is_continue()){
					if (common::env::error.is_break())
						common::env::error.clear();

					break;
				}

				common::env::error.clear();//Continue
			}
			else{//Evaluate 'update'
				update_();
				if (common::env::error.has())
					break;
			}

		} while (iterate && truth_() && !common::env::error.has());
	}
	else//Execute 'else' if any
		value = else_();

	common::env::runtime.storage = previous_storage;//Restore storage
	return (!CSCRIPT_IS(options_, option::value) || common::env::error.has()) ? nullptr : value;
}

cscript::node::control::option cscript::node::control::get_options() const{
	return options_;
}

cscript::object::generic *cscript::node::control::else_(){
	return nullptr;
}

std::nullptr_t cscript::node::control::init_(){
	return nullptr;
}

std::nullptr_t cscript::node::control::update_(){
	return nullptr;
}

bool cscript::node::control::truth_(){
	auto predicate = get_predicate_();
	if (predicate == nullptr)
		return true;

	common::env::runtime = {};
	auto value = predicate->evaluate();
	if (common::env::error.has())
		return false;

	if (value == nullptr)
		return common::env::error.set("void value in expression", predicate->get_index());

	return (value->to_bool() == !CSCRIPT_IS(options_, option::invert));
}

cscript::node::generic::ptr_type cscript::node::control::get_predicate_(){
	return nullptr;
}

cscript::node::generic::ptr_type cscript::node::control::get_body_(){
	return nullptr;
}
