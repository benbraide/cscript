#pragma once

#ifndef CSCRIPT_FUNCTION_ENTRY_H
#define CSCRIPT_FUNCTION_ENTRY_H

#include "basic_function.h"
#include "../storage/generic_storage.h"

namespace cscript{
	namespace function{
		class entry : public basic{
		public:
			typedef std::shared_ptr<definition> definition_type;
			typedef std::list<generic *> list_type;

			typedef std::shared_mutex lock_type;
			typedef std::shared_lock<lock_type> shared_lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			struct parameter_limits{
				int minimum;
				int maximum;
			};

			entry(const std::string &name, storage::generic *storage, type::generic::ptr_type type,
				const parameter_limits &parameter_limits);

			virtual ~entry();

			virtual generic *get_matched(int &score) override;

			virtual generic *get_matched(const type::generic *type) override;

			virtual type::generic::ptr_type get_type() override;

			virtual definition *get_definition() override;

			virtual std::string print() override;

			virtual void set_definition(definition_type definition);

		protected:
			type::generic::ptr_type type_;
			parameter_limits parameter_limits_;
			definition_type definition_;
		};
	}
}

#endif /* !CSCRIPT_FUNCTION_ENTRY_H */
