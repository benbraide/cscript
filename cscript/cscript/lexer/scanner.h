#pragma once

#ifndef CSCRIPT_SCANNER_H
#define CSCRIPT_SCANNER_H

#include "generic_scanner.h"

namespace cscript{
	namespace lexer{
		class scanner : public generic_scanner{
		public:
			explicit scanner(generic_source &source);

			virtual generic_source &branch() override;

			virtual generic_source &unbranch() override;

			virtual generic_source &enable_multi_thread(bool enabled = true) override;

			virtual generic_source &disable_multi_thread() override;

			virtual generic_source &advance_marker(int offset = 1) override;

			virtual token_type next(source_info &info, int count = 1) override;

			virtual token_type peek(source_info &info, int count = 1) override;

			virtual generic_source &ignore(source_info &info, int count = 1) override;

			virtual generic_source &cache(source_info &info, int count = 1) override;

			virtual char get_char(int count = 1) override;

			virtual char peek_char(int count = 1) const override;

			virtual const token::index &get_index() const override;

			virtual bool has_more() const override;

			virtual bool is_branched() const override;

			virtual bool is_multi_threaded() const override;

			virtual int get_cached_size() const override;

			virtual generic_scanner &save(token_type value) override;

		protected:
			generic_source &source_;
		};
	}
}

#endif /* !CSCRIPT_SCANNER_H */
