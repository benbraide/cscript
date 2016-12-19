#include "defined_symbols.h"

void cscript::lexer::defined_symbols::add(const std::string &name, const value_type &value){
	map_[name] = value;
	compile_();
}

void cscript::lexer::defined_symbols::add(const std::string &name, value_type &&value){
	map_[name] = static_cast<value_type &&>(value);
	compile_();
}

void cscript::lexer::defined_symbols::remove(const std::string &name){
	map_.erase(name);
	compile_();
}

const cscript::lexer::defined_symbols::value_type *cscript::lexer::defined_symbols::get_expansion(const std::string &name) const{
	auto entry = look_up_(name);
	return (entry == map_.end()) ? nullptr : &entry->second;
}

bool cscript::lexer::defined_symbols::is_defined(const std::string &name) const{
	return (look_up_(name) != map_.end());
}
