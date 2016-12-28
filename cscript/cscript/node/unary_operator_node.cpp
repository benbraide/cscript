#include "unary_operator_node.h"

#include "../object/boolean_object.h"
#include "../common/env.h"

cscript::node::unary_operator::unary_operator(const lexer::token::index &index, const info_type &info, ptr_type operand, generic *parent)
	: basic(id::operator_, index, parent), info_(info), operand_(operand){}

cscript::node::unary_operator::~unary_operator(){}

cscript::object::generic *cscript::node::unary_operator::evaluate(){
	auto operand = operand_->evaluate();
	if (common::env::error.has())
		return nullptr;

	if (operand == nullptr)
		return common::env::error.set("", index_);

	if (info_.left){
		switch (info_.id){
		case lexer::operator_id::relational_not:
		{
			auto value = !operand->to_bool();
			if (common::env::error.has())
				return nullptr;

			return common::env::temp_storage.add(std::make_shared<object::primitive::boolean>(common::env::temp_address_space,
				value ? type::boolean_value_type::true_ : type::boolean_value_type::false_));
		}
		default:
			break;
		}
	}

	return operand->evaluate(info_);
}

const cscript::node::unary_operator::info_type &cscript::node::unary_operator::get_info() const{
	return info_;
}

cscript::node::generic::ptr_type cscript::node::unary_operator::get_operand() const{
	return operand_;
}

std::string cscript::node::unary_operator::print_() const{
	if (info_.left)
		return (print_open_() + operand_->print() + print_close_());

	return (operand_->print() + print_open_() + print_close_());
}

std::string cscript::node::unary_operator::print_open_() const{
	return info_.value;
}

std::string cscript::node::unary_operator::print_close_() const{
	/*switch (info_.id){
	//case  lexer::operator_id::o
	}*/
	return "";
}
