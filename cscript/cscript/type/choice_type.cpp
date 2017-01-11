#include "choice_type.h"
#include "../object/dynamic_object.h"

cscript::type::choice::choice(ptr_type left, ptr_type right)
	: left_(left), right_(right){}

cscript::type::choice::~choice(){}

const cscript::type::generic *cscript::type::choice::base() const{
	return this;
}

const cscript::type::generic *cscript::type::choice::remove_pointer() const{
	return this;
}

std::string cscript::type::choice::name() const{
	return (left_->name() + " | " + right_->name());
}

std::string cscript::type::choice::print() const{
	return (left_->print() + " | " + right_->print());
}

std::shared_ptr<cscript::object::generic> cscript::type::choice::create(ptr_type this_ptr){
	return std::make_shared<object::dynamic>(this_ptr);
}

std::shared_ptr<cscript::object::generic> cscript::type::choice::create(memory::virtual_address::base_type base, ptr_type this_ptr){
	return std::make_shared<object::dynamic>(base, this_ptr);
}

std::shared_ptr<cscript::object::generic> cscript::type::choice::create_ref(memory::virtual_address::value_type memory_value,
	bool is_constant, ptr_type this_ptr){
	return std::make_shared<object::dynamic_ref>(memory_value, this_ptr, is_constant);
}

cscript::type::id cscript::type::choice::get_id() const{
	return id::choice;
}

cscript::type::generic::size_type cscript::type::choice::get_size() const{
	return 0u;
}

cscript::type::generic::size_type cscript::type::choice::get_composite_size(const generic *type) const{
	return 0u;
}

const cscript::type::generic *cscript::type::choice::get_same(const generic *type) const{
	auto value = right_->get_same(type);
	return (value == nullptr) ? left_->get_same(type) : value;
}

const cscript::type::generic *cscript::type::choice::get_bully(const generic *type) const{
	auto value = right_->get_bully(type);
	return (value == nullptr) ? left_->get_bully(type) : value;
}

int cscript::type::choice::get_score(const generic *type) const{
	if (is_auto() || type->is_auto())
		return 19;

	if (is_any() || type->is_any())
		return 18;

	return is_same(type) ? 20 : 0;
}

bool cscript::type::choice::has_conversion(const generic *type) const{
	return is_same(type);
}

bool cscript::type::choice::is_same(const generic *type) const{
	return right_->is_same(type) ? true : left_->is_same(type);
}

bool cscript::type::choice::is_choice() const{
	return true;
}

bool cscript::type::choice::is_any() const{
	return (right_->is_any() || left_->is_any());
}

bool cscript::type::choice::is_auto() const{
	return (right_->is_auto() || left_->is_auto());
}

bool cscript::type::choice::is_variadic() const{
	return (right_->is_variadic() || left_->is_variadic());
}

bool cscript::type::choice::is_class() const{
	return (right_->is_class() || left_->is_class());
}

bool cscript::type::choice::is_primitive() const{
	return (right_->is_primitive() || left_->is_primitive());
}

bool cscript::type::choice::is_numeric() const{
	return (right_->is_numeric() || left_->is_numeric());
}

bool cscript::type::choice::is_integral() const{
	return (right_->is_integral() || left_->is_integral());
}

bool cscript::type::choice::is_unsigned_integral() const{
	return (right_->is_unsigned_integral() || left_->is_unsigned_integral());
}

bool cscript::type::choice::is_pointer() const{
	return (right_->is_pointer() || left_->is_pointer());
}

bool cscript::type::choice::is_array() const{
	return (right_->is_array() || left_->is_array());
}

bool cscript::type::choice::is_function() const{
	return (right_->is_function() || left_->is_function());
}

bool cscript::type::choice::is_nullptr() const{
	return (right_->is_nullptr() || left_->is_nullptr());
}

cscript::type::generic::ptr_type cscript::type::choice::get_left() const{
	return left_;
}

cscript::type::generic::ptr_type cscript::type::choice::get_right() const{
	return right_;
}
