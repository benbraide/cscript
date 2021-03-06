#pragma once

#ifndef CSCRIPT_GENERIC_SOURCE_H
#define CSCRIPT_GENERIC_SOURCE_H

#include "../common/preprocessor.h"

#include "generic_token_formatter.h"
#include "defined_symbols.h"

namespace cscript{
	namespace lexer{
		class generic_source{
		public:
			enum class option : unsigned int{
				nil					= (0 << 0x0000),
				no_expansion		= (1 << 0x0000),
				invert_skipper		= (1 << 0x0001),
				enable_skipper		= (1 << 0x0002),
				disable_skipper		= (1 << 0x0003),
			};

			typedef generic_token_formatter::token_type token_type;

			virtual ~generic_source(){}

			virtual generic_source &lock() = 0;

			virtual generic_source &unlock() = 0;

			virtual generic_source &branch() = 0;

			virtual generic_source &branch_after(token_type token) = 0;

			virtual generic_source &unbranch() = 0;

			virtual generic_source &enable_multi_thread(bool enabled = true) = 0;

			virtual generic_source &disable_multi_thread() = 0;

			virtual generic_source &advance_marker(int offset = 1) = 0;

			virtual token_type next(source_info &info, int count = 1) = 0;

			virtual token_type peek(source_info &info, int count = 1) = 0;

			virtual token_type peek_after(token_type token, source_info &info, int count = 1) = 0;

			virtual generic_source &ignore(source_info &info, int count = 1) = 0;

			virtual generic_source &ignore_one(token_type token) = 0;

			virtual generic_source &ignore_before(token_id id, source_info &info) = 0;

			virtual generic_source &cache(source_info &info, int count = 1) = 0;

			virtual char get_char(int count = 1) = 0;

			virtual char peek_char(int count = 1) const = 0;

			virtual const token::index &get_index() const = 0;

			virtual bool has_more() const = 0;

			virtual bool is_branched() const = 0;

			virtual bool is_multi_threaded() const = 0;

			virtual int get_cached_size() const = 0;
		};

		class source_guard{
		public:
			typedef std::list<bool> list_type;

			void begin(bool is_rejected){
				list_.push_back(is_rejected);
			}

			void end(){
				if (!list_.empty())
					list_.pop_back();
			}

			bool is_rejected() const{
				return list_.empty() ? false : *list_.rbegin();
			}

			bool is_empty() const{
				return list_.empty();
			}

			list_type::size_type get_count() const{
				return list_.size();
			}

		private:
			list_type list_;
		};

		struct source_info{
		public:
			struct halt_info{
				token_id id;
				std::string value;
			};

			defined_symbols *symbols;
			generic_source *source;
			rule *rule;
			const generic_token_id_compare *skipper;
			const generic_token_formatter *formatter;
			halt_info halt;
			generic_source::option options;
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

			const generic_token_id_compare *get_skipper() const{
				return skipper_;
			}

		private:
			source_info &info_;
			const generic_token_id_compare *skipper_;
		};

		class auto_info{
		public:
			auto_info(source_info &info, const generic_token_id_compare *skipper, const generic_token_formatter *formatter,
				generic_source::option options = generic_source::option::nil)
				: info_(info), skipper_(info.skipper), formatter_(info.formatter), options_(info.options){
				info_.skipper = skipper;
				info_.formatter = formatter;
				info_.options = options;
			}

			~auto_info(){
				info_.skipper = skipper_;
				info_.formatter = formatter_;
				info_.options = options_;
			}

			const generic_token_id_compare *get_skipper() const{
				return skipper_;
			}

			const generic_token_formatter *get_formatter() const{
				return formatter_;
			}

			generic_source::option get_options() const{
				return options_;
			}

		private:
			source_info &info_;
			const generic_token_id_compare *skipper_;
			const generic_token_formatter *formatter_;
			generic_source::option options_;
		};

		inline bool operator ==(const source_info::halt_info &left, const source_info::halt_info &right){
			return (left.id == right.id && left.value == right.value);
		}

		inline bool operator !=(const source_info::halt_info &left, const source_info::halt_info &right){
			return !(left == right);
		}

		CSCRIPT_MAKE_OPERATORS(generic_source::option)
	}
}

#endif /* !CSCRIPT_GENERIC_SOURCE_H */
