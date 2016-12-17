#pragma once

#ifndef CSCRIPT_GENERIC_SOURCE_H
#define CSCRIPT_GENERIC_SOURCE_H

#include "generic_token_formatter.h"

namespace cscript{
	namespace lexer{
		class generic_source{
		public:
			typedef generic_token_formatter::token_type token_type;

			virtual ~generic_source(){}

			virtual generic_source &branch() = 0;

			virtual generic_source &unbranch() = 0;

			virtual generic_source &enable_multi_thread(bool enabled = true) = 0;

			virtual generic_source &disable_multi_thread() = 0;

			virtual generic_source &advance_marker(int offset = 1) = 0;

			virtual token_type next(source_info &info, int count = 1) = 0;

			virtual token_type peek(source_info &info, int count = 1) = 0;

			virtual generic_source &ignore(source_info &info, int count = 1) = 0;

			virtual generic_source &cache(source_info &info, int count = 1) = 0;

			virtual char get_char(int count = 1) = 0;

			virtual char peek_char(int count = 1) const = 0;

			virtual const token::index &get_index() const = 0;

			virtual bool has_more() const = 0;

			virtual bool is_branched() const = 0;

			virtual bool is_multi_threaded() const = 0;

			virtual int get_cached_size() const = 0;
		};

		struct source_info{
			generic_source &source;
			rule &rule;
			const generic_token_id_compare *skipper;
			const generic_token_formatter *formatter;
		};

		class auto_branch{
		public:
			explicit auto_branch(generic_source &source)
				: source_(source){
				source_.branch();
			}

			~auto_branch(){
				source_.unbranch();
			}

		private:
			generic_source &source_;
		};

		class auto_skip{
		public:
			auto_skip(source_info &info, const generic_token_id_compare *skipper)
				: info_(info), skipper_(info.skipper){
				info_.skipper = skipper;
			}

			~auto_skip(){
				info_.skipper = skipper_;
			}

		private:
			source_info &info_;
			const generic_token_id_compare *skipper_;
		};

		class auto_info{
		public:
			auto_info(source_info &info, const generic_token_id_compare *skipper, const generic_token_formatter *formatter)
				: info_(info), skipper_(info.skipper), formatter_(info.formatter){
				info_.skipper = skipper;
				info_.formatter = formatter;
			}

			~auto_info(){
				info_.skipper = skipper_;
				info_.formatter = formatter_;
			}

		private:
			source_info &info_;
			const generic_token_id_compare *skipper_;
			const generic_token_formatter *formatter_;
		};
	}
}

#endif /* !CSCRIPT_GENERIC_SOURCE_H */
