#include "primitive_type.h"

cscript::type::primitive::primitive(id id)
	: id_(id){
	switch (id){
	case type::id::any:
		size_ = static_cast<memory::pool::size_type>(sizeof(memory::virtual_address::value_type));
		break;
	case type::id::bool_:
		size_ = static_cast<memory::pool::size_type>(sizeof(boolean_value_type));
		break;
	case type::id::byte:
		size_ = static_cast<memory::pool::size_type>(sizeof(unsigned char));
		break;
	case type::id::char_:
		size_ = static_cast<memory::pool::size_type>(sizeof(char));
		break;
	case type::id::uchar:
		size_ = static_cast<memory::pool::size_type>(sizeof(unsigned char));
		break;
	case type::id::wchar:
		size_ = static_cast<memory::pool::size_type>(sizeof(wchar_t));
		break;
	case type::id::short_:
		size_ = static_cast<memory::pool::size_type>(sizeof(short));
		break;
	case type::id::ushort:
		size_ = static_cast<memory::pool::size_type>(sizeof(unsigned short));
		break;
	case type::id::int_:
		size_ = static_cast<memory::pool::size_type>(sizeof(int));
		break;
	case type::id::uint:
		size_ = static_cast<memory::pool::size_type>(sizeof(unsigned int));
		break;
	case type::id::long_:
		size_ = static_cast<memory::pool::size_type>(sizeof(long));
		break;
	case type::id::ulong:
		size_ = static_cast<memory::pool::size_type>(sizeof(unsigned long));
		break;
	case type::id::llong:
		size_ = static_cast<memory::pool::size_type>(sizeof(long long));
		break;
	case type::id::ullong:
		size_ = static_cast<memory::pool::size_type>(sizeof(unsigned long long));
		break;
	case type::id::float_:
		size_ = static_cast<memory::pool::size_type>(sizeof(float));
		break;
	case type::id::double_:
		size_ = static_cast<memory::pool::size_type>(sizeof(double));
		break;
	case type::id::ldouble:
		size_ = static_cast<memory::pool::size_type>(sizeof(long double));
		break;
	default:
		size_ = 0;
		break;
	}
}

cscript::type::primitive::~primitive(){}

const cscript::type::generic *cscript::type::primitive::base() const{
	return this;
}

std::string cscript::type::primitive::name() const{
	switch (id_){
	case type::id::any:
		return "any";
	case type::id::auto_:
		return "auto";
	case type::id::bool_:
		return "bool";
	case type::id::bit:
		return "bit";
	case type::id::byte:
		return "byte";
	case type::id::char_:
		return "char";
	case type::id::uchar:
		return "unsigned char";
	case type::id::wchar:
		return "wchar";
	case type::id::short_:
		return "short";
	case type::id::ushort:
		return "unsigned short";
	case type::id::int_:
		return "int";
	case type::id::uint:
		return "unsigned int";
	case type::id::long_:
		return "long";
	case type::id::ulong:
		return "unsigned long";
	case type::id::llong:
		return "long long";
	case type::id::ullong:
		return "unsigned long long";
	case type::id::float_:
		return "float";
	case type::id::double_:
		return "double";
	case type::id::ldouble:
		return "long double";
	default:
		break;
	}

	return "";
}

std::string cscript::type::primitive::print() const{
	return name();
}

cscript::type::id cscript::type::primitive::get_id() const{
	return id_;
}

cscript::type::generic::size_type cscript::type::primitive::get_size() const{
	return size_;
}

cscript::type::generic::size_type cscript::type::primitive::get_composite_size(const generic *type) const{
	if (id_ == type::id::auto_ || is_same(type))
		return type->get_size();

	if (id_ == type::id::any)
		return size_;

	if (!type->is_primitive())
		return 0;

	auto id = type->get_id();
	if (id_ < type::id::char_ || id_ > type::id::ldouble || id < type::id::char_ || id > type::id::ldouble)
		return (id_ == id) ? size_ : 0;

	return (static_cast<int>(id_) < static_cast<int>(id)) ? type->get_size() : size_;
}

const cscript::type::generic *cscript::type::primitive::get_same(const generic *type) const{
	return is_same(type) ? this : nullptr;
}

const cscript::type::generic *cscript::type::primitive::get_bully(const generic *type) const{
	if (id_ == type::id::auto_ || is_same(type))
		return type;

	if (id_ == type::id::any)
		return this;

	if (!type->is_primitive())
		return nullptr;

	auto id = type->get_id();
	if (id_ < type::id::char_ || id_ > type::id::ldouble || id < type::id::char_ || id > type::id::ldouble)
		return nullptr;

	return (static_cast<int>(id_) < static_cast<int>(id)) ? type : this;
}

int cscript::type::primitive::get_score(const generic *type) const{
	if (id_ == type::id::auto_)
		return 19;

	if (id_ == type::id::any)
		return 18;

	if (!type->is_primitive())//Type is not a primitive
		return type->has_conversion(this) ? 18 : 0;

	auto id = type->get_id();
	if (id_ < type::id::char_ || id_ > type::id::ldouble || id < type::id::char_ || id > type::id::ldouble)
		return (id_ == id) ? 20 : 0;

	if (id_ < id)//Narrowing conversion
		return (17 - (static_cast<int>(id) - static_cast<int>(id_)));

	return (id_ == id) ? 20 : 17;//Exact | Widening conversion
}

bool cscript::type::primitive::has_conversion(const generic *type) const{
	if (is_same(type) || id_ == type::id::auto_ || id_ == type::id::any || type->is_any() || type->is_auto())
		return true;

	if (!type->is_primitive())//Type is not a primitive
		return false;

	auto id = type->get_id();
	return !(id_ < type::id::char_ || id_ > type::id::ldouble || id < type::id::char_ || id > type::id::ldouble);
}

bool cscript::type::primitive::is_same(const generic *type) const{
	return (type == this || type->get_id() == id_);
}

bool cscript::type::primitive::is_choice() const{
	return false;
}

bool cscript::type::primitive::is_any() const{
	return (id_ == id::any);
}

bool cscript::type::primitive::is_auto() const{
	return (id_ == id::auto_);
}

bool cscript::type::primitive::is_variadic() const{
	return false;
}

bool cscript::type::primitive::is_class() const{
	return false;
}

bool cscript::type::primitive::is_primitive() const{
	return true;
}

bool cscript::type::primitive::is_numeric() const{
	switch (id_){
	case id::char_:
	case id::uchar:
	case id::short_:
	case id::ushort:
	case id::int_:
	case id::uint:
	case id::long_:
	case id::ulong:
	case id::llong:
	case id::ullong:
	case id::float_:
	case id::double_:
	case id::ldouble:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::type::primitive::is_integral() const{
	switch (id_){
	case id::char_:
	case id::uchar:
	case id::short_:
	case id::ushort:
	case id::int_:
	case id::uint:
	case id::long_:
	case id::ulong:
	case id::llong:
	case id::ullong:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::type::primitive::is_unsigned_integral() const{
	switch (id_){
	case id::uchar:
	case id::ushort:
	case id::uint:
	case id::ulong:
	case id::ullong:
		return true;
	default:
		break;
	}

	return false;
}

bool cscript::type::primitive::is_pointer() const{
	return (id_ == id::pointer);
}
