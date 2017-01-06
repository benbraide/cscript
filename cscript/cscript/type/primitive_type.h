#pragma once

#ifndef CSCRIPT_PRIMITIVE_TYPE_H
#define CSCRIPT_PRIMITIVE_TYPE_H

#include "generic_type.h"

namespace cscript{
	namespace type{
		enum class boolean_value_type{
			nil,
			false_,
			true_,
		};

		class primitive : public generic{
		public:
			explicit primitive(id id);

			virtual ~primitive();

			virtual const generic *base() const override;

			virtual const generic *remove_pointer() const override;

			virtual std::string name() const override;

			virtual std::string print() const override;

			virtual std::shared_ptr<object::generic> create(ptr_type this_ptr) override;

			virtual std::shared_ptr<object::generic> create(memory::virtual_address::base_type base, ptr_type this_ptr) override;

			virtual std::shared_ptr<object::generic> create_ref(memory::virtual_address::value_type memory_value,
				bool is_constant, ptr_type this_ptr) override;

			virtual id get_id() const override;

			virtual size_type get_size() const override;

			virtual size_type get_composite_size(const generic *type) const override;

			virtual const generic *get_same(const generic *type) const override;

			virtual const generic *get_bully(const generic *type) const override;

			virtual int get_score(const generic *type) const override;

			virtual bool has_conversion(const generic *type) const override;

			virtual bool is_same(const generic *type) const override;

			virtual bool is_choice() const override;

			virtual bool is_any() const override;

			virtual bool is_auto() const override;

			virtual bool is_variadic() const override;

			virtual bool is_class()const override;

			virtual bool is_primitive() const override;

			virtual bool is_numeric() const override;

			virtual bool is_integral() const override;

			virtual bool is_unsigned_integral() const override;

			virtual bool is_pointer() const override;
			
		protected:
			id id_;
			size_type size_;
		};

		class pointer : public primitive{
		public:
			explicit pointer(ptr_type value);

			virtual ~pointer();

			virtual const generic *remove_pointer() const override;

			virtual std::string name() const override;

			virtual std::string print() const override;

			virtual std::shared_ptr<object::generic> create(ptr_type this_ptr) override;

			virtual std::shared_ptr<object::generic> create(memory::virtual_address::base_type base, ptr_type this_ptr) override;

			virtual std::shared_ptr<object::generic> create_ref(memory::virtual_address::value_type memory_value,
				bool is_constant, ptr_type this_ptr) override;

			virtual const generic *get_bully(const generic *type) const override;

			virtual int get_score(const generic *type) const override;

			virtual bool has_conversion(const generic *type) const override;

			virtual bool is_same(const generic *type) const override;

			virtual ptr_type get_value();

		protected:
			ptr_type value_;
		};
	}
}

#endif /* !CSCRIPT_PRIMITIVE_TYPE_H */
