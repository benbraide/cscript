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

			base_type get_bound_base(value_type value);

			void copy(value_type destination, generic_type type, value_type source);

			bool copy(value_type destination, value_type source);

			void copy_unchecked(value_type destination, value_type source, size_type size);

			template <typename value_type>
			void write(address_value_type destination, generic_type type, value_type value){
				shared_lock_type guard(lock_);
				auto entry = table::map_.find(destination);
				if (entry != table::map_.end() && entry->second.info.type == type)
					pool::write_unchecked(entry->second.base, value);
				else
					write_unchecked_(destination, value);
			}

			template <typename value_type>
			bool write(address_value_type destination, value_type value){
				auto entry = get_entry(destination);
				if (entry.base == nullptr)
					return false;

				if (entry.size < static_cast<address_value_type>(sizeof(value_type)))
					return false;

				pool::write_unchecked(entry.base, value);
				return true;
			}

			template <typename value_type>
			void write_unchecked(address_value_type destination, value_type value){
				shared_lock_type guard(lock_);
				write_unchecked_<value_type>(destination, value);
			}

			template <typename value_type>
			value_type convert(address_value_type source, generic_type type){
				shared_lock_type guard(lock_);
				auto entry = table::map_.find(source);
				if (entry == table::map_.end() || entry->second.info.type != type)
					return convert_unchecked_<value_type>(source);

				return *reinterpret_cast<value_type *>(entry->second.base);
			}

			template <typename value_type>
			value_type convert_unchecked(address_value_type source){
				shared_lock_type guard(lock_);
				return convert_unchecked_<value_type>(source);
			}

			static const size_type value_type_size = static_cast<size_type>(sizeof(value_type));

		protected:
			value_type find_available_(size_type size);

			void merge_available_(value_type value, size_type size);

			entry get_bound_entry_(value_type value, table::map_type::iterator &offset);

			base_type get_bound_base_(value_type value, table::map_type::iterator &offset);

			void copy_unchecked_(value_type destination, value_type source, size_type size);

			template <typename value_type>
			void write_unchecked_(address_value_type destination, value_type value){
				auto size = sizeof(value_type);
				auto offset = table::map_.begin();

				for (auto source = reinterpret_cast<base_type>(&value); size > 0u; --size, ++source, ++destination){
					auto base = get_bound_base_(destination, offset);
					if (base != nullptr)
						*base = *source;
					else
						break;
				}
			}

			template <typename value_type>
			value_type convert_unchecked_(address_value_type source){
				auto size = sizeof(value_type);
				auto offset = table::map_.begin();

				auto value = value_type();
				for (auto destination = reinterpret_cast<base_type>(&value); size > 0u; --size, ++source, ++destination){
					auto base = get_bound_base_(source, offset);
					if (base != nullptr)
						*destination = *base;
					else
						break;
				}

				return value;
			}

			entry none_{};
			lock_type lock_;
			available_list_type available_list_;
			value_type next_ = 1u;
		};

		CSCRIPT_MAKE_OPERATORS(address_attribute)
	}
}

#endif /* !CSCRIPT_VIRTUAL_ADDRESS_H */
