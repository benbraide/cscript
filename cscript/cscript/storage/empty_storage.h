#pragma once

#ifndef CSCRIPT_EMPTY_STORAGE_H
#define CSCRIPT_EMPTY_STORAGE_H

#include "storage_value.h"

namespace cscript{
	namespace storage{
		class empty : public generic{
		public:
			explicit empty(generic *parent = nullptr);

			virtual ~empty();

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
			value empty_value_;
		};
	}
}

#endif /* !CSCRIPT_EMPTY_STORAGE_H */
