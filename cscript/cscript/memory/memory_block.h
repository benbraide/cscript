#pragma once

#ifndef CSCRIPT_MEMORY_BLOCK_H
#define CSCRIPT_MEMORY_BLOCK_H

#include "memory_pool.h"

namespace cscript{
	namespace memory{
		template <pool::size_type, bool, bool, bool>
		struct memory_block;

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(char)), false, false, false>{
			typedef char target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(unsigned char)), true, false, false>{
			typedef unsigned char target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(wchar_t)), false, false, true>{
			typedef wchar_t target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(short)), false, false, false>{
			typedef short target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(unsigned short)), true, false, false>{
			typedef unsigned short target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(int)), false, false, false>{
			typedef int target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(unsigned int)), true, false, false>{
			typedef unsigned int target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(long)), false, false, true>{
			typedef long target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(unsigned long)), true, false, true>{
			typedef unsigned long target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(long long)), false, false, false>{
			typedef long long target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(unsigned long long)), true, false, false>{
			typedef unsigned long long target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(float)), false, true, false>{
			typedef float target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(double)), false, true, false>{
			typedef double target_type;
		};

		template<>
		struct memory_block<static_cast<pool::size_type>(sizeof(long double)), false, true, true>{
			typedef long double target_type;
		};

		class block_operations{
		public:
			template <typename target_type>
			static inline target_type minus(pool::base_type base){
				return -(*((target_type *)base));
			}

			template <typename target_type>
			static inline target_type bitwise_inverse(pool::base_type base){
				return ~(*((target_type *)base));
			}

			template <typename target_type, typename value_type>
			static inline target_type add(pool::base_type base, value_type value){
				return (*((target_type *)base) + (target_type)value);
			}

			template <typename target_type, typename value_type>
			static inline target_type multiply(pool::base_type base, value_type value){
				return (*((target_type *)base) * (target_type)value);
			}

			template <typename target_type, typename value_type>
			static inline target_type divide(pool::base_type base, value_type value){
				return (*((target_type *)base) / (target_type)value);
			}

			template <typename target_type, typename value_type>
			static inline target_type modulus(pool::base_type base, value_type value){
				return (*((target_type *)base) % (target_type)value);
			}

			template <typename target_type, typename value_type>
			static inline target_type left_shift(pool::base_type base, value_type value){
				return (*((target_type *)base) << (target_type)value);
			}

			template <typename target_type, typename value_type>
			static inline target_type right_shift(pool::base_type base, value_type value){
				return (*((target_type *)base) >> (target_type)value);
			}

			template <typename target_type, typename value_type>
			static inline target_type bitwise_and(pool::base_type base, value_type value){
				return (*((target_type *)base) & (target_type)value);
			}

			template <typename target_type, typename value_type>
			static inline target_type bitwise_or(pool::base_type base, value_type value){
				return (*((target_type *)base) | (target_type)value);
			}

			template <typename target_type, typename value_type>
			static inline target_type bitwise_xor(pool::base_type base, value_type value){
				return (*((target_type *)base) ^ (target_type)value);
			}

			template <typename target_type, typename value_type>
			static inline void assign(target_type(*method)(pool::base_type, value_type), pool::base_type base, pool::base_type left, value_type value){
				*((target_type *)base) = method(left, value);
			}

			template <typename target_type, typename value_type>
			static inline int compare(pool::base_type base, value_type value){
				if (*((target_type *)base) == (target_type)value)
					return 0;

				return (*((target_type *)base) < (target_type)value) ? -1 : 1;
			}
		};
	}
}

#endif /* !CSCRIPT_MEMORY_BLOCK_H */
