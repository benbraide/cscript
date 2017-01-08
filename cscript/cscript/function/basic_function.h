#pragma once

#ifndef CSCRIPT_BASIC_FUNCTION_H
#define CSCRIPT_BASIC_FUNCTION_H

#include <list>
#include <mutex>

#include "function_definition.h"

namespace cscript{
	namespace function{
		class basic : public generic{
		public:
			typedef std::list<generic *> list_type;

			typedef std::shared_mutex lock_type;
			typedef std::shared_lock<lock_type> shared_lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			explicit basic(std::string &name);

			virtual ~basic();

			virtual object::generic *call() override;

			virtual generic &add(generic &value) override;

			virtual generic *get_matched(int &score) override;

			virtual generic *get_matched(const type::generic *type) override;

			virtual type::generic::ptr_type get_type() override;

			virtual definition *get_definition() override;

			virtual std::string print() override;

			virtual const std::string &get_name() const override;

			virtual void set_definition(definition &definition);

		protected:
			std::string *name_;
			list_type list_;
			lock_type lock_;
		};
	}
}

#endif /* !CSCRIPT_BASIC_FUNCTION_H */
