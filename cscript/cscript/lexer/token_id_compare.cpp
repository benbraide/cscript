#include "token_id_compare.h"

cscript::lexer::token_id_compare::token_id_compare(const list_type &list)
	: list_(list){}

cscript::lexer::token_id_compare::token_id_compare(list_type &&list)
	: list_(static_cast<list_type &&>(list)){}

bool cscript::lexer::token_id_compare::is(token_id value) const{
	for (auto entry : list_){
		if (entry == value)
			return true;
	}

	return false;
}

bool cscript::lexer::blank_token_id::is(token_id value) const{
	return (value == token_id::blank || value == token_id::new_line);
}

bool cscript::lexer::literal_token_id::is(token_id value) const{
	switch (value){
	case token_id::exp:
	case token_id::real:
	case token_id::rad:
	case token_id::hex:
	case token_id::dec:
	case token_id::oct:
	case token_id::bin:
	case token_id::quote_dbl:
	case token_id::quote_sng:
	case token_id::quote_back:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::numeric_token_id::is(token_id value) const{
	switch (value){
	case token_id::exp:
	case token_id::real:
	case token_id::rad:
	case token_id::hex:
	case token_id::dec:
	case token_id::oct:
	case token_id::bin:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::integral_token_id::is(token_id value) const{
	switch (value){
	case token_id::rad:
	case token_id::hex:
	case token_id::dec:
	case token_id::oct:
	case token_id::bin:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::real_token_id::is(token_id value) const{
	return (value == token_id::exp || value == token_id::real);
}

bool cscript::lexer::string_token_id::is(token_id value) const{
	switch (value){
	case token_id::quote_dbl:
	case token_id::quote_sng:
	case token_id::quote_back:
	case token_id::esc_quote_dbl:
	case token_id::esc_quote_sng:
	case token_id::esc_quote_back:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::escaped_string_token_id::is(token_id value) const{
	switch (value){
	case token_id::esc_quote_dbl:
	case token_id::esc_quote_sng:
	case token_id::esc_quote_back:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::keyword_token_id::is(token_id value) const{
	switch (value){
	case token_id::class_:
	case token_id::enum_:
	case token_id::namespace_:
	case token_id::using_:
	case token_id::new_:
	case token_id::delete_:
	case token_id::typedef_:
	case token_id::operator_:
	case token_id::sizeof_:
	case token_id::typeof:
	case token_id::echo:
	case token_id::true_:
	case token_id::false_:
	case token_id::auto_:
	case token_id::any:
	case token_id::void_:
	case token_id::bool_:
	case token_id::bit:
	case token_id::byte:
	case token_id::nan:
	case token_id::char_:
	case token_id::wchar:
	case token_id::short_:
	case token_id::int_:
	case token_id::long_:
	case token_id::unsigned_:
	case token_id::float_:
	case token_id::double_:
	case token_id::string:
	case token_id::array_:
	case token_id::try_:
	case token_id::catch_:
	case token_id::finally_:
	case token_id::throw_:
	case token_id::if_:
	case token_id::else_:
	case token_id::unless:
	case token_id::while_:
	case token_id::for_:
	case token_id::until:
	case token_id::do_:
	case token_id::switch_:
	case token_id::case_:
	case token_id::default_:
	case token_id::break_:
	case token_id::continue_:
	case token_id::return_:
	case token_id::decltype_:
	case token_id::function:
	case token_id::nullptr_:
	case token_id::pointer:
	case token_id::tls:
	case token_id::static_:
	case token_id::const_:
	case token_id::final_:
	case token_id::ref:
	case token_id::private_:
	case token_id::protected_:
	case token_id::public_:
	case token_id::type_cast:
	case token_id::self:
	case token_id::placeholder:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::context_keyword_token_id::is(token_id value) const{
	switch (value){
	case token_id::default_:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::block_token_id::is(token_id value) const{
	switch (value){
	case token_id::class_:
	case token_id::enum_:
	case token_id::namespace_:
	case token_id::try_:
	case token_id::catch_:
	case token_id::finally_:
	case token_id::if_:
	case token_id::else_:
	case token_id::switch_:
	case token_id::do_:
	case token_id::while_:
	case token_id::for_:
	case token_id::until:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::storage_token_id::is(token_id value) const{
	switch (value){
	case token_id::class_:
	case token_id::enum_:
	case token_id::namespace_:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::type_token_id::is(token_id value) const{
	switch (value){
	case token_id::auto_:
	case token_id::any:
	case token_id::void_:
	case token_id::bool_:
	case token_id::bit:
	case token_id::byte:
	case token_id::char_:
	case token_id::wchar:
	case token_id::short_:
	case token_id::int_:
	case token_id::long_:
	case token_id::unsigned_:
	case token_id::float_:
	case token_id::double_:
	case token_id::string:
	case token_id::array_:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::control_token_id::is(token_id value) const{
	switch (value){
	case token_id::try_:
	case token_id::catch_:
	case token_id::finally_:
	case token_id::if_:
	case token_id::else_:
	case token_id::switch_:
	case token_id::do_:
	case token_id::while_:
	case token_id::for_:
	case token_id::until:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::group_token_id::is(token_id value) const{
	switch (value){
	case token_id::open_sq:
	case token_id::open_par:
	case token_id::open_cur:
	case token_id::close_sq:
	case token_id::close_par:
	case token_id::close_cur:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::skip_token_id::is(token_id value) const{
	switch (value){
	case token_id::blank:
	case token_id::new_line:
	case token_id::comment_sng:
	case token_id::comment_mult:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::single_line_skip_token_id::is(token_id value) const{
	switch (value){
	case token_id::blank:
	case token_id::comment_sng:
	case token_id::comment_mult:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::preprocessor_token_id::is(token_id value) const{
	switch (value){
	case token_id::prep_incl:
	case token_id::prep_def:
	case token_id::prep_undef:
	case token_id::prep_ifdef:
	case token_id::prep_ifndef:
	case token_id::prep_else:
	case token_id::prep_endif:
	case token_id::prep_pragma:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::lexer::end_guard_token_id::is(token_id value) const{
	switch (value){
	case token_id::prep_else:
	case token_id::prep_endif:
		return true;
	default:
		break;
	}

	return false;
}

cscript::lexer::aggregate::aggregate(const list_type &list)
	: list_(list){}

cscript::lexer::aggregate::aggregate(list_type &&list)
	: list_(static_cast<list_type &&>(list)){}

bool cscript::lexer::aggregate::is(token_id value) const{
	for (auto entry : list_){
		if (entry->is(value))
			return true;
	}

	return false;
}

const cscript::lexer::blank_token_id cscript::lexer::token_id_compare_collection::blank;

const cscript::lexer::literal_token_id cscript::lexer::token_id_compare_collection::literal;

const cscript::lexer::numeric_token_id cscript::lexer::token_id_compare_collection::numeric;

const cscript::lexer::integral_token_id cscript::lexer::token_id_compare_collection::integral;

const cscript::lexer::real_token_id cscript::lexer::token_id_compare_collection::real;

const cscript::lexer::string_token_id cscript::lexer::token_id_compare_collection::string;

const cscript::lexer::escaped_string_token_id cscript::lexer::token_id_compare_collection::escaped_string;

const cscript::lexer::keyword_token_id cscript::lexer::token_id_compare_collection::keyword;

const cscript::lexer::context_keyword_token_id cscript::lexer::token_id_compare_collection::context_keyword;

const cscript::lexer::block_token_id cscript::lexer::token_id_compare_collection::block;

const cscript::lexer::storage_token_id cscript::lexer::token_id_compare_collection::storage;

const cscript::lexer::type_token_id cscript::lexer::token_id_compare_collection::type;

const cscript::lexer::control_token_id cscript::lexer::token_id_compare_collection::control;

const cscript::lexer::group_token_id cscript::lexer::token_id_compare_collection::group;

const cscript::lexer::skip_token_id cscript::lexer::token_id_compare_collection::skip;

const cscript::lexer::single_line_skip_token_id cscript::lexer::token_id_compare_collection::single_line_skip;

const cscript::lexer::preprocessor_token_id cscript::lexer::token_id_compare_collection::preprocessor;

const cscript::lexer::end_guard_token_id cscript::lexer::token_id_compare_collection::end_guard;
