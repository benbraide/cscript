#pragma once

#ifndef CSCRIPT_BASIC_STORAGE_H
#define CSCRIPT_BASIC_STORAGE_H

#include <mutex>
#include <list>
#include <map>

#include "storage_value.h"

#include "../common/table.h"
#include "../node/operator_value_node.h"
#include "../function/generic_function.h"

namespace cscript{
	namespace storage{
		class basic : public generic, public common::table<value>{
		public:
			typedef std::list<function_type> list_type;

			typedef std::shared_mutex lock_type;
			typedef std::shared_lock<lock_type> shared_lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			explicit basic(generic *parent = nullptr);

			virtual ~basic();

			virtual void store_function(function_type object) override;

			virtual generic *get_parent() override;

			virtual generic *get_matched(const std::string &key) override;

			virtual generic_value *find(const std::string &key) override;

			virtual generic_value &add(const std::string &key) override;

			virtual bool remove(const std::string &key) override;

			virtual bool remove(generic_value &value) override;

			virtual generic &use(const std::string &key, generic_value &value) override;

			virtual generic &use(generic &storage) override;

			virtual generic &traverse(traverser_type traverser) override;

		protected:
			generic *parent_;
			list_type function_list_;
			lock_type lock_;
		};
	}
}

#endif /* !CSCRIPT_BASIC_STORAGE_H */
