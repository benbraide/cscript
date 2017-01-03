#pragma once

#ifndef CSCRIPT_BASIC_STORAGE_H
#define CSCRIPT_BASIC_STORAGE_H

#include <mutex>
#include <map>

#include "storage_value.h"

#include "../common/table.h"
#include "../node/operator_value_node.h"

namespace cscript{
	namespace storage{
		class basic : public generic, public common::table<value>{
		public:
			typedef std::recursive_mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;
			typedef std::map<operator_key, value> operator_list_type;

			explicit basic(generic *parent = nullptr);

			virtual ~basic();

			virtual generic *get_parent() override;

			virtual generic *get_matched(const std::string &key) override;

			virtual generic_value *find(const std::string &key) override;

			virtual generic_value *find(const operator_key &key) override;

			virtual generic_value &add(const std::string &key) override;

			virtual generic_value &add(const operator_key &key) override;

			virtual bool remove(const std::string &key) override;

			virtual bool remove(const operator_key &key) override;

			virtual bool remove(generic_value &value) override;

			virtual generic &use(const std::string &key, generic_value &value) override;

			virtual generic &use(const operator_key &key, generic_value &value) override;

			virtual generic &use(generic &storage) override;

			virtual generic &traverse(traverser_type traverser) override;

		protected:
			generic *parent_;
			lock_type lock_;
			operator_list_type operator_list_;
		};
	}
}

#endif /* !CSCRIPT_BASIC_STORAGE_H */
