#pragma once

#ifndef CSCRIPT_TOKEN_H
#define CSCRIPT_TOKEN_H

#include <string>
#include <boost/any.hpp>

namespace cscript{
	namespace lexer{
		class token{
		public:
			template <class integral_type>
			struct string_to_number{
				static integral_type convert(const std::string &value, int base){
					return static_cast<integral_type>(std::stoi(value, nullptr, base));
				}
			};

			template <>
			struct string_to_number<long>{
				static long convert(const std::string &value, int base){
					return std::stol(value, nullptr, base);
				}
			};

			template <>
			struct string_to_number<unsigned long>{
				static unsigned long convert(const std::string &value, int base){
					return std::stoul(value, nullptr, base);
				}
			};

			template <>
			struct string_to_number<long long>{
				static long long convert(const std::string &value, int base){
					return std::stoll(value, nullptr, base);
				}
			};

			template <>
			struct string_to_number<unsigned long long>{
				static unsigned long long convert(const std::string &value, int base){
					return std::stoull(value, nullptr, base);
				}
			};

			template <>
			struct string_to_number<float>{
				static float convert(const std::string &value){
					return std::stof(value);
				}
			};

			template <>
			struct string_to_number<double>{
				static double convert(const std::string &value){
					return std::stod(value);
				}
			};

			template <>
			struct string_to_number<long double>{
				static long double convert(const std::string &value){
					return std::stold(value);
				}
			};

			struct index{
				int line;
				int column;
			};

			struct adjustment{
				int left;
				int right;
			};

			token();

			token(const index &index, const std::string &value, int match_index, const adjustment &adjustment);

			token(const index &index, std::string &&value, int match_index, const adjustment &adjustment);

			virtual ~token();

			virtual bool is_processed() const;

			token &update(const index &index, const std::string &value, int match_index);

			token &update(const index &index, std::string &&value, int match_index);

			token &update(const index &index);

			token &update(const std::string &value);

			token &update(std::string &&value);

			token &update(int match_index);

			token &set_adjustment(const adjustment &value);

			const index &get_index() const;

			const adjustment &get_adjustment() const;

			const std::string &get_value() const;

			std::string get_adjusted_value() const;

			int get_line() const;

			int get_column() const;
			
			int get_match_index() const;

			std::string to_string() const;

			bool has_adjustments() const;

			template <typename integral_type>
			integral_type get_numerical_value(int base = 10) const{
				return string_to_number<integral_type>::convert(value_, base);
			}

			template <typename object_type>
			object_type *query(){
				return dynamic_cast<object_type *>(this);
			}

			template <typename object_type>
			const object_type *query() const{
				return dynamic_cast<const object_type *>(this);
			}

			static std::string index_to_string(const index &value);

		private:
			index index_;
			std::string value_;
			int match_index_;
			adjustment adjustment_{};
		};

		class preprocessed_token : public token{
		public:
			template <typename value_type>
			preprocessed_token(const index &index, const std::string &value, int match_index,
				const adjustment &adjustment, const value_type &processed_value)
				: token(index, value, match_index, adjustment), processed_value_(processed_value),
				value_type_info_(typeid(value_type)){}

			template <typename value_type>
			preprocessed_token(const index &index, const std::string &value, int match_index,
				const adjustment &adjustment, value_type &&processed_value)
				: token(index, static_cast<std::string &&>(value), match_index, adjustment),
				processed_value_(static_cast<value_type &&>(processed_value)), value_type_info_(typeid(value_type)){}

			virtual bool is_processed() const override;

			const std::type_info &get_type_info() const;

			const boost::any &get_processed_value() const;

			template <typename value_type>
			const value_type &get_converted_processed_value() const{
				return boost::any_cast<const value_type &>(processed_value_);
			}

			template <typename value_type>
			bool is() const{
				return (typeid(value_type) == value_type_info_);
			}

		private:
			boost::any processed_value_;
			std::type_info value_type_info_;
		};

		class error_token : public token{
		public:
			error_token(const index &index, const std::string &value);

			error_token(const index &index, std::string &&value);
		};
	}
}

#endif /* !CSCRIPT_TOKEN_H */
