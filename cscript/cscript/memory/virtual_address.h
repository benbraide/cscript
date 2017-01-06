#pragma once

#ifndef CSCRIPT_VIRTUAL_ADDRESS_H
#define CSCRIPT_VIRTUAL_ADDRESS_H

#include <shared_mutex>
#include <memory>

#include "../common/preprocessor.h"
#include "../common/table.h"
#include "../storage/generic_storage.h"

#include "memory_pool.h"

namespace cscript{
	namespace object{
		class generic;
	}

	namespace type{
		class generic;
	}

	namespace memory{
		typedef unsigned __int64 address_value_type;
		typedef std::shared_ptr<type::generic> address_generic_type;

		enum class address_attribute : unsigned int{
			nil				= (0 << 0x0000),
			temp			= (1 << 0x0000),
			ref				= (1 << 0x0001),
			indirect		= (1 << 0x0002),
			uninitialized	= (1 << 0x0003),
			constant		= (1 << 0x0004),
			is_nan			= (1 << 0x0005),
			byte_aligned	= (1 << 0x0006),
			final_			= (1 << 0x0007),
			static_			= (1 << 0x0008),
			private_		= (1 << 0x0009),
			protected_		= (1 << 0x000A),
			no_dealloc		= (1 << 0x000B),
			is_child		= (1 << 0x000C),
			const_target	= (1 << 0x000D),
		};

		struct address_entry_info{
			object::generic *object;
			address_generic_type type;
			storage::generic *storage;
		};

		struct address_entry{
			pool::base_type base;
			pool::size_type size;
			address_value_type value;
			address_attribute attributes;
			address_entry_info info;
		};

		class virtual_address : public common::table<address_entry, address_value_type>{
		public:
			typedef address_value_type value_type;
			typedef address_attribute attribute;
			typedef address_entry_info entry_info;
			typedef address_entry entry;
			typedef address_generic_type generic_type;

			typedef pool::base_type base_type;
			typedef pool::size_type size_type;

			typedef std::shared_mutex lock_type;
			typedef std::shared_lock<lock_type> shared_lock_type;
			typedef std::lock_guard<lock_type> guard_type;
			typedef std::map<value_type, size_type> available_list_type;

			virtual_address();

			virtual ~virtual_address();

			value_type add(size_type size, bool allocate = true);

			template <typename type>
			value_type add(bool allocate = true){
				return add(static_cast<size_type>(sizeof(type)), allocate);
			}

			bool remove(value_type value);

			entry &get_entry(value_type value);

			entry get_bound_entry(value_type value);

			bool copy(value_type destination, value_type source);

			void copy_unchecked(value_type destination, value_type source, size_type size);

			template <typename value_type>
			bool write(value_type destination, value_type value){
				auto entry = get_entry(destination);
				if (entry.base == nullptr)
					return false;

				if (entry.size < static_cast<address_value_type>(sizeof(value_type)))
					return false;

				pool::write_unchecked(entry.base, value);
				return true;
			}

			template <typename value_type>
			void write_unchecked(value_type destination, value_type value){
				auto size = sizeof(value_type);
				for (auto source = reinterpret_cast<base_type>(&value); size > 0u; --size, ++source, ++destination){
					auto destination_entry = get_bound_entry(destination);
					*destination_entry.base = *source;
				}
			}

			template <typename value_type>
			static bool write_ref(value_type destination, const value_type &value){
				auto entry = get_entry(destination);
				if (entry.base == nullptr)
					return false;

				if (entry.size < static_cast<address_value_type>(sizeof(value_type)))
					return false;

				pool::write_ref_unchecked(entry.base, value);
				return true;
			}

			static const size_type value_type_size = static_cast<size_type>(sizeof(value_type));

		protected:
			value_type find_available_(size_type size);

			void merge_available_(value_type value, size_type size);

			entry none_{};
			lock_type lock_;
			available_list_type available_list_;
			value_type next_ = 1u;
		};

		CSCRIPT_MAKE_OPERATORS(address_attribute)
	}
}

#endif /* !CSCRIPT_VIRTUAL_ADDRESS_H */
