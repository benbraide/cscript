#pragma once

#ifndef CSCRIPT_NUMERIC_OBJECT_H
#define CSCRIPT_NUMERIC_OBJECT_H

#include "boolean_object.h"

namespace cscript{
	namespace object{
		namespace primitive{
			class numeric : public basic{
			public:
				typedef std::shared_ptr<memory::block_operator> block_operator_type;

				explicit numeric(const type::generic::ptr_type type);

				template <typename value_type>
				numeric(const type::generic::ptr_type type, value_type value)
					: numeric(common::env::address_space, type, value){
					CSCRIPT_REMOVE(memory_.attributes, memory::virtual_address::attribute::uninitialized);
				}

				numeric(memory::virtual_address &address_space, const type::generic::ptr_type type);

				template <typename value_type>
				numeric(memory::virtual_address &address_space, const type::generic::ptr_type type, value_type value)
					: basic(address_space.add<value_type>()){
					memory_.type = type;
					memory::pool::write_unchecked(memory_.base, value);
					CSCRIPT_REMOVE(memory_.attributes, memory::virtual_address::attribute::uninitialized);
				}

				numeric(memory::virtual_address::entry &parent, const type::generic::ptr_type type);

				virtual ~numeric();

				virtual generic *clone() override;

				virtual generic *cast(const type::generic *type) override;

				virtual generic *evaluate(const binary_info &info) override;

				virtual generic *evaluate(const unary_info &info) override;

				virtual bool is_nan();

				template <typename value_type>
				void cast_value(){
					switch (memory_.type->get_id()){
					case type::id::char_:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<char>(memory_.base));
						break;
					case type::id::uchar:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<unsigned char>(memory_.base));
						break;
					case type::id::short_:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<short>(memory_.base));
						break;
					case type::id::ushort:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<unsigned short>(memory_.base));
						break;
					case type::id::int_:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<int>(memory_.base));
						break;
					case type::id::uint:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<unsigned int>(memory_.base));
						break;
					case type::id::long_:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<long>(memory_.base));
						break;
					case type::id::ulong:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<unsigned long>(memory_.base));
						break;
					case type::id::llong:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<long long>(memory_.base));
						break;
					case type::id::ullong:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<unsigned long long>(memory_.base));
						break;
					case type::id::float_:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<float>(memory_.base));
						break;
					case type::id::double_:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<double>(memory_.base));
						break;
					case type::id::ldouble:
						common::env::static_block1.get<value_type>() =
							static_cast<value_type>(memory::pool::convert_unchecked<long double>(memory_.base));
						break;
					default:
						break;
					}
				}

				template <typename value_type>
				value_type get_value(){
					cast_value<value_type>();
					return common::env::static_block1.get<value_type>();
				}

				template <typename value_type>
				int compare(value_type value){
					if (is_uninitialized()){
						common::env::error.set("Uninitialized object in expression");
						return -2;
					}

					auto op = get_op_();
					memory::pool::convert_unchecked<value_type>(common::env::static_block1.get_base()) = value;

					return (op->compare(memory_.base, common::env::static_block1.get_base()) != 0);
				}

			protected:
				virtual ptr_type clone_(const type::generic::ptr_type type, bool is_nan);

				virtual generic *create_boolean_(boolean::value_type value);

				virtual generic *plus_();

				virtual generic *minus_();

				virtual generic *bitwise_inverse_();

				virtual generic *add_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual generic *subtract_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual generic *multiply_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual generic *divide_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual generic *modulus_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual generic *left_shift_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual generic *right_shift_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual generic *bitwise_and_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual generic *bitwise_or_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual generic *bitwise_xor_(type::generic::ptr_type bully, numeric &operand, bool assign);

				virtual int compare_(type::generic::ptr_type bully, numeric &operand);

				virtual block_operator_type prepare_(type::generic::ptr_type bully, numeric &operand, bool assign,
					generic *&destination, memory::pool::base_type &left, memory::pool::base_type &right);

				block_operator_type get_op_();

				static block_operator_type cast_value_(type::id id, numeric &source);
			};
		}
	}
}

#endif /* !CSCRIPT_NUMERIC_OBJECT_H */
