#include "iteration_node.h"
#include "../common/env.h"

cscript::node::for_iteration::for_iteration(const lexer::token::index &index, ptr_type init, ptr_type predicate,
	ptr_type update, ptr_type body, ptr_type else_node)
	: control(id::for_, index, option::iterate), init_node_(init), predicate_(predicate), body_(body), update_node_(update), else_node_(else_node){}

cscript::node::for_iteration::~for_iteration(){}

cscript::node::generic::ptr_type cscript::node::for_iteration::clone(){
	auto init_clone = (init_node_ == nullptr) ? nullptr : init_node_->clone();
	auto body_clone = (body_ == nullptr) ? nullptr : body_->clone();
	auto update_clone = (update_node_ == nullptr) ? nullptr : update_node_->clone();
	auto predicate_clone = (predicate_ == nullptr) ? nullptr : predicate_->clone();
	auto else_clone = (else_node_ == nullptr) ? nullptr : else_node_->clone();

	return std::make_shared<for_iteration>(index_, init_clone, predicate_clone, body_clone, update_clone, else_clone);
}

cscript::node::generic::ptr_type cscript::node::for_iteration::get_init(){
	return init_node_;
}

cscript::node::generic::ptr_type cscript::node::for_iteration::get_predicate(){
	return predicate_;
}

cscript::node::generic::ptr_type cscript::node::for_iteration::get_body(){
	return body_;
}

cscript::node::generic::ptr_type cscript::node::for_iteration::get_update(){
	return update_node_;
}

cscript::node::generic::ptr_type cscript::node::for_iteration::get_else_node(){
	return else_node_;
}

std::string cscript::node::for_iteration::print_() const{
	auto init_print = (init_node_ == nullptr) ? nullptr : init_node_->print();
	auto update_print = (update_node_ == nullptr) ? nullptr : update_node_->print();
	auto predicate_print = (predicate_ == nullptr) ? nullptr : predicate_->print();

	return ("for (" + init_print + "; " + predicate_print + "; " + update_print + "){ ... }");
}

cscript::object::generic *cscript::node::for_iteration::else_(){
	if (else_node_ == nullptr)
		return nullptr;

	object::generic *value = nullptr;
	auto statements = else_node_->query<collection>();

	if (statements != nullptr){
		for (auto statement : statements->get_list()){
			value = statement->evaluate();
			common::env::temp_storage.clear();
			common::env::runtime.operand.constant_objects.clear();
			if (common::env::error.has())
				break;
		}
	}
	else//Single
		value = else_node_->evaluate();

	return value;
}

std::nullptr_t cscript::node::for_iteration::init_(){
	if (init_node_ != nullptr)
		init_node_->evaluate();

	return nullptr;
}

std::nullptr_t cscript::node::for_iteration::update_(){
	if (update_node_ != nullptr)
		update_node_->evaluate();

	return nullptr;
}

cscript::node::generic::ptr_type cscript::node::for_iteration::get_predicate_(){
	return predicate_;
}

cscript::node::generic::ptr_type cscript::node::for_iteration::get_body_(){
	return body_;
}
