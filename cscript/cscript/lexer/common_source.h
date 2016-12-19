#pragma once

#ifndef CWIN_COMMON_SOURCE_H
#define CWIN_COMMON_SOURCE_H

#include <list>
#include <mutex>
#include <memory>

#include "generic_source.h"

namespace cscript{
	namespace lexer{
		namespace source{
			class source_cache{
			public:
				typedef generic_source::token_type token_type;
				typedef std::list<token_type> list_type;

				typedef list_type::size_type size_type;
				typedef list_type::iterator iterator_type;

				typedef std::list<size_type> size_list_type;

				source_cache()
					: offset_(list_.begin()){}

				void clear(){
					if (offset_ != list_.end()){
						list_.erase(offset_, list_.end());
						offset_ = std::next(list_.begin(), offset_value_);
					}
				}

				void branch(){
					branches_.push_back(offset_value_);
					offset_value_ = list_.size();
					offset_ = list_.end();
				}

				void unbranch(){
					if (!branches_.empty()){
						offset_value_ = *branches_.rbegin();
						offset_ = std::next(list_.begin(), offset_value_);
						branches_.pop_back();
					}
				}

				void insert(token_type value, int count){
					if (static_cast<int>(this->size()) <= count){
						list_.push_back(value);
						offset_ = std::next(list_.begin(), offset_value_);
					}
					else//Insert
						list_.insert(std::next(list_.begin(), offset_value_ + count), value);
				}

				void append(token_type value){
					list_.push_back(value);
					if (offset_ == list_.end())
						offset_ = std::next(list_.begin(), offset_value_);
				}

				void append(const list_type &value){
					for (auto entry : value)
						list_.push_back(entry);

					if (offset_ == list_.end())
						offset_ = std::next(list_.begin(), offset_value_);
				}

				token_type get(int &count, bool discard, source_info &info){
					if (count <= 0 || offset_ == list_.end())
						return nullptr;

					token_type value;
					iterator_type end;

					if (info.skipper == nullptr){
						if (static_cast<int>(size()) < count){
							value = nullptr;
							end = list_.end();
							count -= static_cast<int>(list_.size());
						}
						else{//Within range
							value = *(end = std::next(offset_, count - 1));
							count = 0;
							++end;
						}
					}
					else{//Check for skips
						for (auto iter = (end = offset_); iter != list_.end(); ++iter, ++end){
							if (info.skipper->is(info.rule.map_index((*iter)->get_match_index())) ==
								CSCRIPT_IS(info.options, generic_source::option::invert_skipper)){
								if (--count <= 0){
									++end;
									value = *iter;
									break;
								}
							}
						}
					}

					if (discard){//Erase range
						list_.erase(offset_, end);
						offset_ = std::next(list_.begin(), offset_value_);
					}

					return value;
				}

				token_type get(int &count, source_info *info) const{
					if (count <= 0 || offset_ == list_.end())
						return nullptr;

					token_type value;
					if (info != nullptr && info->skipper == nullptr){
						if (static_cast<int>(size()) < count){
							value = nullptr;
							count -= static_cast<int>(list_.size());
						}
						else{//Within range
							value = *std::next(offset_, count - 1);
							count = 0;
						}
					}
					else{//Check for skips
						for (auto iter = offset_; iter != list_.end(); ++iter){
							if (info->skipper->is(info->rule.map_index((*iter)->get_match_index())) ==
								CSCRIPT_IS(info->options, generic_source::option::invert_skipper)){
								if (--count <= 0){
									value = *iter;
									break;
								}
							}
						}
					}

					return value;
				}

				size_type size() const{
					return (list_.size() - offset_value_);
				}

				bool is_branched() const{
					return !branches_.empty();
				}

				bool is_empty() const{
					return list_.empty();
				}

			private:
				list_type list_;
				size_list_type branches_;
				iterator_type offset_;
				size_type offset_value_ = 0;
			};

			template <class iterator_type>
			class common : public generic_source{
			public:
				typedef iterator_type iterator_type;

				typedef std::recursive_mutex lock_type;
				typedef std::shared_ptr<lock_type> lock_type_ptr;

				class auto_lock{
				public:
					explicit auto_lock(lock_type_ptr value)
						: value_(value){
						if (value_ != nullptr)
							value_->lock();
					}

					~auto_lock(){
						if (value_ != nullptr)
							value_->unlock();
					}

				private:
					lock_type_ptr value_;
				};

				virtual ~common(){}

				virtual generic_source &branch() override{
					cache_.branch();
					return *this;
				}

				virtual generic_source &unbranch() override{
					cache_.unbranch();
					return *this;
				}

				virtual generic_source &enable_multi_thread(bool enabled = true) override{
					if (enabled){
						if (lock_ == nullptr)
							lock_ = std::make_shared<lock_type>();
					}
					else//Disable
						lock_ = nullptr;

					return *this;
				}

				virtual generic_source &disable_multi_thread() override{
					lock_ = nullptr;
					return *this;
				}

				virtual generic_source &advance_marker(int offset = 1) override{
					if (offset < 0)
						current_ -= static_cast<source_cache::size_type>(-offset);
					else//Forward
						current_ += static_cast<source_cache::size_type>(offset);

					index_.column += offset;
					return *this;
				}

				virtual token_type next(source_info &info, int count = 1) override{
					return next_(info, count, false);
				}

				virtual token_type peek(source_info &info, int count = 1) override{
					return next_(info, count, true);
				}

				virtual generic_source &ignore(source_info &info, int count = 1) override{
					next_(info, count, false);
					return *this;
				}

				virtual generic_source &cache(source_info &info, int count = 1) override{
					next_(info, count, true);
					return *this;
				}

				virtual char get_char(int count = 1) override{
					auto value = '\0', next = '\0';
					for (auto i = 0; i < count && current_ < end_; ++i){
						if ((value = get_char_()) == '\r'){
							if ((next = peek_char(1)) == '\n')// \r\n
								get_char_();//Ignore

							value = '\n';
							update_markers_();
						}
						else if (value == '\n'){
							if ((next = peek_char(1)) == '\r')// \n\r
								get_char_();//Ignore

							value = '\n';
							update_markers_();
						}
						else//Advance column
							++index_.column;
					}

					return value;
				}

				virtual char peek_char(int count = 1) const override{
					auto target = current_ + (count - 1);
					return (target < end_) ? *target : '\0';
				}

				virtual const token::index &get_index() const override{
					auto count = 1;
					return cache_.is_empty() ? index_ : cache_.get(count, nullptr)->get_index();
				}

				virtual bool has_more() const override{
					return (current_ != end_ || !cache_.is_empty());
				}

				virtual bool is_branched() const override{
					return cache_.is_branched();
				}

				virtual bool is_multi_threaded() const override{
					return (lock_ != nullptr);
				}

				virtual int get_cached_size() const override{
					return static_cast<int>(cache_.size());
				}

			protected:
				virtual char get_char_(){
					return *(current_++);
				}

				virtual token_type next_(source_info &info, int count, bool cache){
					auto_lock lock(lock_);
					if (count <= 0 || !has_more())//No more tokens
						return nullptr;

					auto cache_offset = count;
					auto value = cache_.get(count, !cache, info);
					if (value != nullptr)//Found in cache
						return value;

					generic_token_formatter::creator_type creator = nullptr;
					generic_rule::match_info match_info{};
					
					token_id id;
					generic_token_formatter::match_info formatted_info;
					const defined_symbols::value_type *expansion;
					
					while (count > 0){
						if (!info.rule.match(current_, end_, match_info))
							return std::make_shared<error_token>(index_, "Unrecognized character encountered!");

						id = info.rule.map_index(match_info.index);
						formatted_info = generic_token_formatter::match_info{
							index_,
							&*current_,
							match_info.size,
							match_info.index
						};

						if (!CSCRIPT_IS(info.options, generic_source::option::no_expansion) && id == token_id::identifier){
							index_.column += static_cast<int>(match_info.size);
							expansion = info.symbols.get_expansion(std::string(formatted_info.start, formatted_info.size));
							if (expansion != nullptr){//Token expanded
								current_ += match_info.size;
								if (!expansion->empty())
									cache_.append(*expansion);

								return next_(info, cache ? cache_offset : count, cache);
							}
						}
						else if (id == token_id::new_line)
							update_markers_();
						else//Advance column
							index_.column += static_cast<int>(match_info.size);

						current_ += match_info.size;
						if (info.formatter != nullptr){//Pass to formatter
							cache_.branch();
							creator = info.formatter->format(formatted_info, info);
							id = info.rule.map_index(formatted_info.match_index);
							cache_.unbranch();
						}

						if (cache){//Cache
							create_value_(info, formatted_info, creator, value);
							cache_.insert(value, cache_offset++);
						}

						if (formatted_info.match_index == info.rule.get_error_index()){//Error encountered
							if (value == nullptr)
								create_value_(info, formatted_info, creator, value);
							return value;
						}

						if (info.skipper == nullptr || (info.skipper->is(id) ==
							CSCRIPT_IS(info.options, generic_source::option::invert_skipper))){//Don't skip value
							if (value == nullptr && count == 1)
								create_value_(info, formatted_info, creator, value);
							--count;
						}
					}

					return value;
				}

				virtual void update_markers_(){
					++index_.line;
					index_.column = 1;
				}

				virtual void create_value_(source_info &info, generic_token_formatter::match_info &formatted_info,
					generic_token_formatter::creator_type creator, token_type &value){
					if (creator == nullptr){//Create default
						value = std::make_shared<token>(formatted_info.index, std::string(formatted_info.start, formatted_info.size),
							formatted_info.match_index, formatted_info.adjustment);
					}
					else//Use creator
						value = creator(formatted_info);
				}

				token::index index_{ 1, 1 };
				iterator_type current_;
				iterator_type end_;
				source_cache cache_;
				lock_type_ptr lock_;
			};
		}
	}
}

#endif /* !CWIN_COMMON_SOURCE_H */
