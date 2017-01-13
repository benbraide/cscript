#include "external_function_definition.h"

cscript::function::external_definition::external_definition(handler_type handler)
	: handler_(handler){}

cscript::function::external_definition::~external_definition(){}

cscript::object::generic *cscript::function::external_definition::call(storage::generic *storage){
	return (handler_ == nullptr) ? nullptr : handler_(storage);
}
