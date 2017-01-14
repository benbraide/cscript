#include "internal_functions.h"
#include "env.h"

#include "../object/type_object.h"
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
	if (!CSCRIPT_IS(entry.attributes, memory::address_attribute::allocated))
		return env::error.set("Cannot deallocate an unallocated memory");

	if (size_value < static_cast<int>(entry.size)){//Shrink
		if (!env::address_space.shrink(value, entry.size - size_value))
			return env::error.set("Memory reallocation failure");
	}
	else if (static_cast<int>(entry.size) < size_value && !env::address_space.extend(value, size_value - entry.size))//Extend
		return env::error.set("Memory reallocation failure");

	return env::temp_storage.add(std::make_shared<object::primitive::numeric>(env::ullong_type, value));
}

cscript::object::generic *cscript::common::internal_functions::is_alloc(storage::generic *storage){
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

	auto base = env::address_space.get_bound_base(numeric->get_value<memory::address_value_type>());
	return env::temp_storage.add(std::make_shared<object::primitive::boolean>(
		(base != 0u) ? type::boolean_value_type::true_ : type::boolean_value_type::false_));
}

cscript::object::generic *cscript::common::internal_functions::is_block_head(storage::generic *storage){
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

cscript::object::generic *cscript::common::internal_functions::get_block_size(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto numeric = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (numeric == nullptr || !numeric->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto &entry = env::address_space.get_entry(numeric->get_value<memory::address_value_type>());
	if (entry.value == 0u)
		return env::error.set("Target memory is invalid or not block head");

	return env::temp_storage.add(std::make_shared<object::primitive::numeric>(env::uint_type, entry.size));
}

cscript::object::generic *cscript::common::internal_functions::get_block_object(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto numeric = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (numeric == nullptr || !numeric->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto &entry = env::address_space.get_entry(numeric->get_value<memory::address_value_type>());
	if (entry.value == 0u)
		return env::error.set("Target memory is invalid or not block head");

	if (entry.info.object == nullptr)
		return env::error.set("Target memory has no object");

	return entry.info.object;
}

cscript::object::generic *cscript::common::internal_functions::get_block_type(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto numeric = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (numeric == nullptr || !numeric->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto &entry = env::address_space.get_entry(numeric->get_value<memory::address_value_type>());
	if (entry.value == 0u)
		return env::error.set("Target memory is invalid or not block head");

	if (entry.info.type == nullptr)
		return env::error.set("Target memory has no type");

	return env::temp_storage.add(std::make_shared<object::primitive::type_object>(entry.info.type));
}

cscript::object::generic *cscript::common::internal_functions::copy(storage::generic *storage){
	validate_arguments_(3);
	if (env::error.has())
		return nullptr;

	auto source = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (source == nullptr || !source->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto size = (*std::next(env::runtime.arguments.begin()))->query<object::primitive::numeric>();
	if (size == nullptr || !size->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto destination = (*env::runtime.arguments.rbegin())->query<object::primitive::numeric>();
	if (destination == nullptr || !destination->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto size_value = size->get_value<int>();
	if (size_value <= 0)
		return env::error.set("Cannot reallocate requested memory block");

	auto source_value = source->get_value<memory::address_value_type>();
	auto destination_value = destination->get_value<memory::address_value_type>();

	env::address_space.copy_unchecked(destination_value, source_value, size_value);
	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::set(storage::generic *storage){
	validate_arguments_(3);
	if (env::error.has())
		return nullptr;

	auto address = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (address == nullptr || !address->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto size = (*std::next(env::runtime.arguments.begin()))->query<object::primitive::numeric>();
	if (size == nullptr || !size->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto target = (*env::runtime.arguments.rbegin())->query<object::primitive::numeric>();
	if (target == nullptr || !target->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto size_value = size->get_value<int>();
	if (size_value < 0)
		return env::error.set("Cannot write requested memory block");

	auto address_value = address->get_value<memory::address_value_type>();
	if (size_value == 0){
		if (env::address_space.get_entry(address_value).value == 0u)
			return env::error.set("Target memory is invalid or not block head");
		env::address_space.set(address_value, target->get_value<int>());
	}
	else
		env::address_space.set_unchecked(address_value, size_value, target->get_value<int>());

	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::move(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::watch(storage::generic *storage){
	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::unwatch(storage::generic *storage){
	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::string_new(storage::generic *storage){
	validate_arguments_(1);
	if (env::error.has())
		return nullptr;

	auto size = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (size == nullptr || !size->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto size_value = size->get_value<int>();
	if (size_value <= 0)
		return env::error.set("Cannot reallocate requested memory block");

	return env::temp_storage.add(env::create_string(std::string(size_value, ' ')));
}

cscript::object::generic *cscript::common::internal_functions::string_resize(storage::generic *storage){
	validate_arguments_(2);
	if (env::error.has())
		return nullptr;

	auto numeric = (*env::runtime.arguments.begin())->query<object::primitive::numeric>();
	if (numeric == nullptr || !numeric->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto size = (*env::runtime.arguments.rbegin())->query<object::primitive::numeric>();
	if (size == nullptr || !size->get_type()->is_integral())
		return env::error.set("Invalid parameter in function call");

	auto size_value = size->get_value<int>();
	if (size_value < 0)
		return env::error.set("Cannot resize requested string");

	auto value = numeric->get_value<memory::address_value_type>();
	if (!CSCRIPT_IS(env::address_space.get_entry(value).attributes, memory::address_attribute::string_))
		return env::error.set("String operation on non-string memory");

	auto &string = env::address_space.get_string(value);
	if (size_value == static_cast<int>(string.size()))
		return env::temp_storage.add(std::make_shared<object::pointer>(value));

	auto string_copy = string;
	string_copy.resize(size_value);
	return env::temp_storage.add(env::create_string(string_copy));
}

cscript::object::generic *cscript::common::internal_functions::string_insert(storage::generic *storage){
	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::string_erase(storage::generic *storage){
	return nullptr;
}

cscript::object::generic *cscript::common::internal_functions::string_clear(storage::generic *storage){
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
