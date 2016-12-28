#include "parser_context.h"

cscript::parser::context::basic::basic(generic_context *parent)
	: parent_(parent){}

cscript::parser::context::basic::~basic(){

}

cscript::parser::generic_context *cscript::parser::context::basic::get_parent(){
	return parent_;
}

bool cscript::parser::context::basic::is_block() const{
	return false;
}

cscript::parser::context::expression::expression(generic_context *parent)
	: basic(parent), precedence_(-1){}

cscript::parser::context::expression::expression(int precedence, generic_context *parent)
	: basic(parent), precedence_(precedence){}

bool cscript::parser::context::expression::accepts(node::id id) const{
	return true;
}

int cscript::parser::context::expression::get_precedence() const{
	return precedence_;
}

bool cscript::parser::context::expression::is_stackable(int precedence, bool left_associative){
	if (precedence_ == -1)
		return true;

	if (precedence == precedence_)
		return !left_associative;

	return (precedence_ < precedence);
}
