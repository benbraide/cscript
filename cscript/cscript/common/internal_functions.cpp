#include "internal_functions.h"
#include "env.h"

#include "../object/static_array_object.h"

cscript::object::generic *cscript::common::internal_functions::alloc(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto numeric = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (numeric == nullptr || !numeric->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto value = numeric->get_value<int>();
	if (value <= 0)
		return env::error.set("Cannot allocate requested memory block");

	return env::temp_storage.add(std::make_shared<object::primitive::numeric>(env::ullong_type,
		env::address_space.allocate(value)));
}

cscript::object::generic *cscript::common::internal_functions::dealloc(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto numeric = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (numeric == nullptr || !numeric->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto &entry = env::address_space.get_entry(numeric->get_value<memory::address_value_type>());
	if (!CSCRIPT_IS(entry.attributes, memory::address_attribute::allocated))
		return env::error.set("Cannot deallocate an unallocated memory");

	if (!env::address_space.remove(entry.value))
		return env::error.set("Memory deallocation failure");

	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::realloc(storage::generic *storage){
	validate_arguments_(2);
	if (env::error.has())
		return nullptr;

	auto address = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (address == nullptr || !address->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto size = (*env::runtime.arguments.rbegin())->query<object::primitive::numeric>();
	if (size == nullptr || !size->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto size_value = size->get_value<int>();
	if (size_value <= 0)
		return env::error.set("Cannot reallocate requested memory block");

	auto value = address->get_value<memory::address_value_type>();
	auto &entry = env::address_space.get_entry(value);
	if (size_value < static_cast<int>(entry.size)){//Shrink
		if (!env::address_space.shrink(value, size_value))
			return env::error.set("Memory reallocation failure");
	}
	else if (static_cast<int>(entry.size) < size_value && !env::address_space.extend(value, size_value))//Extend
		return env::error.set("Memory reallocation failure");

	return env::temp_storage.add(std::make_shared<object::primitive::numeric>(env::ullong_type, value));
}

cscript::object::generic * cscript::common::internal_functions::is_alloc(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto numeric = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (numeric == nullptr || !numeric->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto &entry = env::address_space.get_entry(numeric->get_value<memory::address_value_type>());
	auto value = CSCRIPT_IS(entry.attributes, memory::address_attribute::allocated);

	return env::temp_storage.add(std::make_shared<object::primitive::boolean>(
		value ? type::boolean_value_type::true_ : type::boolean_value_type::false_));
}

cscript::object::generic *cscript::common::internal_functions::is_valid_address(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto numeric = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (numeric == nullptr || !numeric->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto &entry = env::address_space.get_entry(numeric->get_value<memory::address_value_type>());
	return env::temp_storage.add(std::make_shared<object::primitive::boolean>(
		(entry.value != 0u) ? type::boolean_value_type::true_ : type::boolean_value_type::false_));
}

cscript::object::generic *cscript::common::internal_functions::move(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::count(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto static_array = (*env::runtime.arguments.begin())->query<object::primitive::static_array>();
	if (static_array != nullptr){
		return env::temp_storage.add(std::make_shared<object::primitive::numeric>(env::uint_type,
			static_cast<unsigned int>(static_array->get_entries().size())));
	}

	return env::error.set("Invalid parameter in function call");
}

cscript::object::generic *cscript::common::internal_functions::begin(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto static_array = (*env::runtime.arguments.begin())->query<object::primitive::static_array>();
	if (static_array != nullptr){
		return env::temp_storage.add(std::make_shared<object::pointer>(
			static_array->get_memory_value() + 1, env::ullong_type));
	}

	return env::error.set("Invalid parameter in function call");
}

cscript::object::generic *cscript::common::internal_functions::end(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto static_array = (*env::runtime.arguments.begin())->query<object::primitive::static_array>();
	if (static_array != nullptr){
		return env::temp_storage.add(std::make_shared<object::pointer>(
			static_array->get_memory_value() + static_array->get_type()->get_size(), env::ullong_type));
	}

	return env::error.set("Invalid parameter in function call");
}

cscript::object::generic *cscript::common::internal_functions::distance(storage::generic *storage){
	validate_arguments_(2);
	if (env::error.has())
		return nullptr;

	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::advance(storage::generic *storage){
	validate_arguments_(2);
	if (env::error.has())
		return nullptr;

	return nullptr;
}

std::nullptr_t cscript::common::internal_functions::validate_arguments_(int count){
	if (static_cast<int>(env::runtime.arguments.size()) != count)
		return env::error.set("Invalid number of arguments in function call");

	for (auto argument : env::runtime.arguments){
		if (argument == nullptr)
			return env::error.set("void value in expression");
	}

	return nullptr;
}
