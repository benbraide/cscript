#pragma once

#ifndef CSCRIPT_VIRTUAL_ADDRESS_H
#define CSCRIPT_VIRTUAL_ADDRESS_H

#include <list>
#include <mutex>
#include <memory>

#include "../common/preprocessor.h"
#include "memory_pool.h"

namespace cscript{
	namespace object{
		class generic;
	}

	namespace type{
		class generic;
	}

	namespace memory{
		class virtual_address{
		public:
			typedef unsigned __int64 value_type;

			typedef pool::base_type base_type;
			typedef pool::size_type size_type;

			typedef std::mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;
			typedef std::shared_ptr<type::generic> generic_type;

			enum class attribute : unsigned int{
				nil				= (0 << 0x0000),
			};

			struct entry{
				virtual_address *address;
				base_type base;
				size_type size;
				value_type value;
				size_type offset;
				attribute attributes;
				object::generic *object;
				generic_type type;
			};

			typedef std::list<entry> list_type;
			typedef std::list<list_type> multi_list_type;

			typedef list_type::iterator iterator_type;
			typedef list_type::const_iterator const_iterator_type;

			struct find_info{
				iterator_type iterator;
				list_type *list;
			};

			explicit virtual_address(pool &pool);

			~virtual_address();

			entry &add(size_type size, attribute set = attribute::nil, attribute remove = attribute::nil);

			template <typename type>
			entry &add(attribute set = attribute::nil, attribute remove = attribute::nil){
				return add(static_cast<size_type>(sizeof(type)), set, remove);
			}

			bool remove(const entry &entry);

			bool update_ref_count(const entry &entry, size_type count, bool increment);

			bool decrement_ref_count(const entry &entry);

			bool increment_ref_count(const entry &entry);

			bool is_none(const entry &entry) const;

			template <typename value_type>
			static bool write(virtual_address::value_type address, const value_type &value){
				return write(address, get_entry(address), value);
			}

			template <typename value_type>
			static bool write(virtual_address::value_type address, const entry &entry, const value_type &value){
				if (is_none(entry) || entry.base == nullptr)
					return false;

				auto offset = entry.value - address;
				if ((entry.size - offset) < static_cast<virtual_address::value_type>(sizeof(value_type)))
					return false;

				pool::write_unchecked(entry.base + offset, value);
				return true;
			}

			template <typename value_type>
			static void write_unchecked(virtual_address::value_type address, const value_type &value){
				write_unchecked(address, get_entry(address), value);
			}

			template <typename value_type>
			static void write_unchecked(virtual_address::value_type address, const entry &entry, const value_type &value){
				if (is_none(entry) || entry.base == nullptr)
					return;

				auto offset = entry.value - address, end = entry.value + entry.size;
				auto size = static_cast<virtual_address::value_type>(sizeof(value_type));
				auto value_base = reinterpret_cast<const char *>(&value);

				for (auto base = entry.base + offset; base != nullptr && size > 0u; --size, ++value_base){
					*base = *value_base;
					if (end <= ++offset){//Passed address boundary
						auto &next = entry.address->get_entry(offset);
						if (is_none(next))
							break;

						offset = 0;
						end = next.value + next.size;
						base = next.base;
					}
					else//Advance pointer
						++base;
				}
			}

		private:
			bool remove_(find_info &info);

			void find_(const entry &entry, find_info &info);

			void find_available_(size_type size, find_info &info);

			list_type *get_list_(size_type offset);

			list_type *get_next_list_(size_type &offset);

			pool &pool_;
			multi_list_type list_;
			entry none_{};
			mutable lock_type lock_;
		};

		CSCRIPT_MAKE_OPERATORS(virtual_address::attribute)
	}
}

#endif /* !CSCRIPT_VIRTUAL_ADDRESS_H */
