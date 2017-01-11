#pragma once

#ifndef CSCRIPT_SELECTION_NODE_H
#define CSCRIPT_SELECTION_NODE_H

#include "control_node.h"
#include "collection_node.h"

namespace cscript{
	namespace node{
		template <class env_type, id id, control::option options>
		class selection : public control{
		public:
			selection(const lexer::token::index &index, ptr_type predicate, ptr_type body, ptr_type else_node = nullptr)
				: control(id, index, options), predicate_(predicate), body_(body), else_node_(else_node){}

			virtual ~selection(){}

			virtual ptr_type clone() override{
				if (else_node_ == nullptr){
					if (body_ == nullptr)
						return std::make_shared<selection>(index_, predicate_->clone(), nullptr, nullptr);

					return std::make_shared<selection>(index_, predicate_->clone(), body_->clone(), nullptr);
				}

				return std::make_shared<selection>(index_, predicate_->clone(), body_->clone(), else_node_->clone());
			}

			virtual ptr_type get_predicate(){
				return predicate_;
			}

			virtual ptr_type get_body(){
				return body_;
			}

			virtual ptr_type get_else_node(){
				return else_node_;
			}

		protected:
			virtual std::string print_() const override{
				if (id == node::id::conditional)
					return (predicate_->print() + " ? " + body_->print() + " : " + else_node_->print());

				if (id == node::id::do_while)
					return ("do{...}(" + predicate_->print() + ")");

				std::string value;
				switch (id){
				case node::id::unless:
					value = "unless";
					break;
				case node::id::while_:
					value = "while";
					break;
				case node::id::until:
					value = "until";
					break;
				default:
					value = "if";
					break;
				}

				return (value + " (" + predicate_->print() + "){...}");
			}

			virtual object::generic *else_() override{
				if (else_node_ == nullptr)
					return nullptr;

				object::generic *value = nullptr;
				auto statements = else_node_->query<collection>();

				if (statements != nullptr){
					for (auto statement : statements->get_list()){
						value = statement->evaluate();
						env_type::temp_storage.clear();
						env_type::runtime.operand.constant_objects.clear();
						if (env_type::error.has())
							break;
					}
				}
				else//Single
					value = else_node_->evaluate();

				return value;
			}

			virtual ptr_type get_predicate_() override{
				return predicate_;
			}

			virtual ptr_type get_body_() override{
				return body_;
			}

			ptr_type predicate_;
			ptr_type body_;
			ptr_type else_node_;
		};

		template <class env_type>
		using if_selection = selection<env_type, id::if_, control::option::nil>;

		template <class env_type>
		using unless_selection = selection<env_type, id::unless, control::option::invert>;

		template <class env_type>
		using conditional_selection = selection<env_type, id::conditional, control::option::value>;
	}
}

#endif /* !CSCRIPT_SELECTION_NODE_H */
