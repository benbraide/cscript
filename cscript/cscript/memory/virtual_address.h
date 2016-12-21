#pragma once

#ifndef CSCRIPT_VIRTUAL_ADDRESS_H
#define CSCRIPT_VIRTUAL_ADDRESS_H

#include <list>
#include <mutex>

#include "../common/preprocessor.h"
#include "memory_pool.h"

namespace cscript{
	namespace object{
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

			enum class attribute : unsigned int{
				nil				= (0 << 0x0000),
			};

			struct entry{
				virtual_address *address;
				base_type base;
				size_type size;
				value_type value;
				attribute attributes;
				object::generic *object;
			};

			typedef std::list<entry> list_type;
			typedef list_type::iterator iterator_type;
			typedef list_type::const_iterator const_iterator_type;

			explicit virtual_address(pool &pool, attribute attributes = attribute::nil);

			~virtual_address();

			entry &add(size_type size);

			template <typename type>
			entry &add(){
				return add(static_cast<size_type>(sizeof(type)));
			}

			bool remove(value_type value);

			bool update_ref_count(value_type value, size_type count, bool increment);

			bool decrement_ref_count(value_type value);

			bool increment_ref_count(value_type value);

			bool is_none(const entry &entry) const;

			bool has(value_type value) const;

			entry &get_entry(value_type value);

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
			iterator_type find_(value_type value);

			const_iterator_type find_(value_type value) const;

			iterator_type find_available_(size_type size);

			pool &pool_;
			entry *last_ = nullptr;
			list_type list_;
			entry none_{};
			attribute attributes_;
			mutable lock_type lock_;
		};

		CSCRIPT_MAKE_OPERATORS(virtual_address::attribute)
	}
}

#endif /* !CSCRIPT_VIRTUAL_ADDRESS_H */
