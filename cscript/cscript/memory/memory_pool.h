#pragma once

#ifndef CSCRIPT_MEMORY_POOL_H
#define CSCRIPT_MEMORY_POOL_H

#include <map>

namespace cscript{
	namespace memory{
		class static_block{
		public:
			explicit static_block(int size = 128)
				: base_(new char[size]){}

			~static_block(){
				if (base_ != nullptr){
					delete[] base_;
					base_ = nullptr;
				}
			}

			char *get_base(){
				return base_;
			}

			template <typename value_type>
			value_type &get(){
				return *reinterpret_cast<value_type *>(base_);
			}

		private:
			char *base_;
		};

		class pool{
		public:
			typedef char *base_type;
			typedef unsigned int size_type;

			template <typename value_type>
			static bool write(base_type base, size_type size, value_type value){
				if (size != static_cast<size_type>(sizeof(value_type)))
					return false;

				convert_unchecked<value_type>(base) = value;
				return true;
			}

			template <typename value_type>
			static void write_unchecked(base_type base, value_type value){
				convert_unchecked<value_type>(base) = value;
			}

			template <typename value_type>
			static bool write_ref(base_type base, size_type size, const value_type &value){
				if (size != static_cast<size_type>(sizeof(value_type)))
					return false;

				convert_unchecked<value_type>(base) = value;
				return true;
			}

			template <typename value_type>
			static void write_ref_unchecked(base_type base, const value_type &value){
				convert_unchecked<value_type>(base) = value;
			}

			template <typename value_type>
			static value_type *convert(base_type base, size_type size){
				if (size != static_cast<size_type>(sizeof(value_type)))
					return nullptr;

				return reinterpret_cast<value_type *>(base);
			}

			template <typename value_type>
			static value_type &convert_unchecked(base_type base){
				return *reinterpret_cast<value_type *>(base);
			}
		};

		class block_operator{
		public:
			virtual ~block_operator(){}

			virtual void plus(char *destination, char *right) = 0;

			virtual void minus(char *destination, char *right) = 0;

			virtual void bitwise_inverse(char *destination, char *right) = 0;

			virtual void add(char *destination, char *left, char *right) = 0;

			virtual void subtract(char *destination, char *left, char *right) = 0;

			virtual void multiply(char *destination, char *left, char *right) = 0;

			virtual void divide(char *destination, char *left, char *right) = 0;

			virtual void modulus(char *destination, char *left, char *right) = 0;

			virtual void left_shift(char *destination, char *left, char *right) = 0;

			virtual void right_shift(char *destination, char *left, char *right) = 0;

			virtual void bitwise_and(char *destination, char *left, char *right) = 0;

			virtual void bitwise_or(char *destination, char *left, char *right) = 0;

			virtual void bitwise_xor(char *destination, char *left, char *right) = 0;

			virtual int compare(char *left, char *right) = 0;
		};

		template <class value_type>
		class typed_block_operator : public block_operator{
		public:
			virtual void plus(char *destination, char *right) override{
				pool::convert_unchecked<value_type>(destination) = pool::convert_unchecked<value_type>(right);
			}

			virtual void minus(char *destination, char *right) override{
				minus_<value_type>(destination, right);
			}

			virtual void bitwise_inverse(char *destination, char *right) override{
				bitwise_inverse_<value_type>(destination, right);
			}

			virtual void add(char *destination, char *left, char *right) override{
				pool::convert_unchecked<value_type>(destination) =
					(pool::convert_unchecked<value_type>(left) + pool::convert_unchecked<value_type>(right));
			}

			virtual void subtract(char *destination, char *left, char *right) override{
				pool::convert_unchecked<value_type>(destination) =
					(pool::convert_unchecked<value_type>(left) - pool::convert_unchecked<value_type>(right));
			}

			virtual void multiply(char *destination, char *left, char *right) override{
				pool::convert_unchecked<value_type>(destination) =
					(pool::convert_unchecked<value_type>(left) * pool::convert_unchecked<value_type>(right));
			}

			virtual void divide(char *destination, char *left, char *right) override{
				pool::convert_unchecked<value_type>(destination) =
					(pool::convert_unchecked<value_type>(left) / pool::convert_unchecked<value_type>(right));
			}

			virtual void modulus(char *destination, char *left, char *right) override{
				modulus_<value_type>(destination, left, right);
			}

			virtual void left_shift(char *destination, char *left, char *right) override{
				left_shift_<value_type>(destination, left, right);
			}

			virtual void right_shift(char *destination, char *left, char *right) override{
				right_shift_<value_type>(destination, left, right);
			}

			virtual void bitwise_and(char *destination, char *left, char *right) override{
				bitwise_and_<value_type>(destination, left, right);
			}

			virtual void bitwise_or(char *destination, char *left, char *right) override{
				bitwise_or_<value_type>(destination, left, right);
			}

			virtual void bitwise_xor(char *destination, char *left, char *right) override{
				bitwise_xor_<value_type>(destination, left, right);
			}

			virtual int compare(char *left, char *right) override{
				auto &left_value = pool::convert_unchecked<value_type>(left);
				auto &right_value = pool::convert_unchecked<value_type>(right);

				if (left_value == right_value)
					return 0;

				return (left_value < right_value) ? -1 : 1;
			}

		private:
			template <typename value_type>
			std::enable_if_t<!std::is_integral<value_type>::value || std::is_signed<value_type>::value> minus_(char *destination, char *right){
				pool::convert_unchecked<value_type>(destination) = -pool::convert_unchecked<value_type>(right);
			}

			template <typename value_type>
			std::enable_if_t<std::is_integral<value_type>::value && !std::is_signed<value_type>::value> minus_(char *destination, char *right){}

			template <typename value_type>
			std::enable_if_t<std::is_integral<value_type>::value> bitwise_inverse_(char *destination, char *right){
				pool::convert_unchecked<value_type>(destination) = ~pool::convert_unchecked<value_type>(right);
			}

			template <typename value_type>
			std::enable_if_t<!std::is_integral<value_type>::value> bitwise_inverse_(char *destination, char *right){}

			template <typename value_type>
			std::enable_if_t<std::is_integral<value_type>::value> modulus_(char *destination, char *left, char *right){
				pool::convert_unchecked<value_type>(destination) =
					pool::convert_unchecked<value_type>(left) % pool::convert_unchecked<value_type>(right);
			}

			template <typename value_type>
			std::enable_if_t<!std::is_integral<value_type>::value> modulus_(char *destination, char *left, char *right){}

			template <typename value_type>
			std::enable_if_t<std::is_integral<value_type>::value> left_shift_(char *destination, char *left, char *right){
				pool::convert_unchecked<value_type>(destination) =
					pool::convert_unchecked<value_type>(left) << pool::convert_unchecked<value_type>(right);
			}

			template <typename value_type>
			std::enable_if_t<!std::is_integral<value_type>::value> left_shift_(char *destination, char *left, char *right){}

			template <typename value_type>
			std::enable_if_t<std::is_integral<value_type>::value> right_shift_(char *destination, char *left, char *right){
				pool::convert_unchecked<value_type>(destination) =
					pool::convert_unchecked<value_type>(left) >> pool::convert_unchecked<value_type>(right);
			}

			template <typename value_type>
			std::enable_if_t<!std::is_integral<value_type>::value> right_shift_(char *destination, char *left, char *right){}

			template <typename value_type>
			std::enable_if_t<std::is_integral<value_type>::value> bitwise_and_(char *destination, char *left, char *right){
				pool::convert_unchecked<value_type>(destination) =
					pool::convert_unchecked<value_type>(left) & pool::convert_unchecked<value_type>(right);
			}

			template <typename value_type>
			std::enable_if_t<!std::is_integral<value_type>::value> bitwise_and_(char *destination, char *left, char *right){}

			template <typename value_type>
			std::enable_if_t<std::is_integral<value_type>::value> bitwise_or_(char *destination, char *left, char *right){
				pool::convert_unchecked<value_type>(destination) =
					pool::convert_unchecked<value_type>(left) | pool::convert_unchecked<value_type>(right);
			}

			template <typename value_type>
			std::enable_if_t<!std::is_integral<value_type>::value> bitwise_or_(char *destination, char *left, char *right){}

			template <typename value_type>
			std::enable_if_t<std::is_integral<value_type>::value> bitwise_xor_(char *destination, char *left, char *right){
				pool::convert_unchecked<value_type>(destination) =
					pool::convert_unchecked<value_type>(left) ^ pool::convert_unchecked<value_type>(right);
			}

			template <typename value_type>
			std::enable_if_t<!std::is_integral<value_type>::value> bitwise_xor_(char *destination, char *left, char *right){}
		};
	}
}

#endif /* !CSCRIPT_MEMORY_POOL_H */
