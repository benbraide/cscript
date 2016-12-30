#pragma once

#ifndef CSCRIPT_NAMED_STORAGE_H
#define CSCRIPT_NAMED_STORAGE_H

#include "basic_storage.h"

namespace cscript{
	namespace storage{
		class named : public basic{
		public:
			explicit named(const std::string &name, generic *parent = nullptr);

			virtual ~named();

			virtual generic *get_matched(const std::string &key) override;

			virtual const std::string &get_name() const;

		protected:
			std::string name_;
		};
	}
}

#endif /* !CSCRIPT_NAMED_STORAGE_H */
