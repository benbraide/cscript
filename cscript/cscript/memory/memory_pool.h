#pragma once

#ifndef CSCRIPT_MEMORY_POOL_H
#define CSCRIPT_MEMORY_POOL_H

#include <map>

namespace cscript{
	namespace memory{
		class pool{
		public:
			typedef char *base_type;
			typedef unsigned int size_type;

			struct info{
				size_type size;
				size_type ref_count;
			};

			typedef std::map<base_type, info> list_type;

			~pool(){
				for (auto &entry : list_)
					delete[] entry.first;
			}

			base_type add(size_type size){
				auto base = new char[size];
				if (base != nullptr)
					list_[base] = info{ size, 1 };
				return base;
			}

			template <typename type>
			base_type add(){
				return add(static_cast<size_type>(sizeof(type)));
			}

			bool remove(const base_type base){
				auto entry = list_.find(base);
				if (entry == list_.end())
					return false;

				list_.erase(entry);
				return true;
			}

			int update_ref_count(const base_type base, size_type count, bool increment){
				auto entry = list_.find(base);
				if (entry == list_.end())
					return 0;

				if (!increment){//Decrement
					if (entry->second.ref_count <= count){//All references dropped
						delete[] base;
						list_.erase(entry);
						return 2;
					}

					entry->second.ref_count -= count;
				}
				else//Increment
					entry->second.ref_count += count;

				return 1;
			}

			int decrement_ref_count(const base_type base){
				return update_ref_count(base, 1, false);
			}

			int increment_ref_count(const base_type base){
				return update_ref_count(base, 1, true);
			}

			const info &get_info(const base_type base) const{
				auto entry = list_.find(base);
				return (entry == list_.end()) ? none_ : entry->second;
			}

			info &get_mutable_info(const base_type base){
				auto entry = list_.find(base);
				return (entry == list_.end()) ? none_ : entry->second;
			}

			bool is_none(const info &info) const{
				return (&info == &none_);
			}

			bool has(const base_type base) const{
				return (list_.find(base) != list_.end());
			}

			template <typename value_type>
			static bool write(base_type base, const value_type &value){
				return write(base, get_info(base), value);
			}

			template <typename value_type>
			static bool write(base_type base, const info &info, const value_type &value){
				if (is_none(info) || info.size != static_cast<size_type>(sizeof(value_type)))
					return false;

				convert_unchecked<value_type>(base) = value;
				return true;
			}

			template <typename value_type>
			static void write_unchecked(base_type base, const value_type &value){
				convert_unchecked<value_type>(base) = value;
			}

			template <typename value_type>
			static value_type *convert(base_type base){
				return convert(base, get_info(base));
			}

			template <typename value_type>
			static value_type *convert(base_type base, const info &info){
				if (is_none(info) || info.size != static_cast<size_type>(sizeof(value_type)))
					return nullptr;

				return reinterpret_cast<value_type *>(base);
			}

			template <typename value_type>
			static value_type &convert_unchecked(base_type base){
				return *reinterpret_cast<value_type *>(base);
			}

		private:
			list_type list_;
			info none_{};
		};
	}
}

#endif /* !CSCRIPT_MEMORY_POOL_H */
