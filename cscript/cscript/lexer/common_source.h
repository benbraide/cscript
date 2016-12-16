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

				void append(token_type value){
					list_.push_back(value);
					if (offset_ == list_.end())
						offset_ = std::next(list_.begin(), offset_value_);
				}

				token_type get(int count, bool discard){
					if (offset_ == list_.end())
						return nullptr;

					token_type value;
					iterator_type end;

					if (list_.size() <= (offset_value_ + count - 1)){
						value = nullptr;
						end = list_.end();
					}
					else{//Within range
						value = *(end = std::next(offset_, count - 1));
						++end;
					}

					if (discard)//Erase range
						list_.erase(offset_, end);

					return value;
				}

				token_type get(int count) const{
					if (offset_ == list_.end())
						return nullptr;

					token_type value;
					if (list_.size() <= (offset_value_ + count - 1))
						value = nullptr;
					else//Within range
						value = *std::next(offset_, count);

					return value;
				}

				size_type size() const{
					return branches_.size();
				}

				bool is_branched() const{
					return !branches_.empty();
				}

				bool is_empty() const{
					return branches_.empty();
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
					}

					return value;
				}

				virtual char peek_char(int count = 1) const override{
					auto target = current_ + (count - 1);
					return (target < end_) ? *target : '\0';
				}

				virtual const token::index &get_index() const override{
					return cache_.is_empty() ? index_ : cache_.get(1)->get_index();
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
					if (!has_more())//No more tokens
						return nullptr;

					auto value = cache_.get(count, !cache);//Check in cache
					if (value != nullptr)//Found in cache
						return value;

					generic_rule::match_info match_info{};
					if (!info.rule.match(current_, end_, match_info)){

					}

					auto index = index_;
					auto id = info.rule.map_index(match_info.index);
					std::string matched_value(current_, current_ + match_info.size);

					current_ += match_info.size;
					switch (id){
					case token_id::identifier://Try expanding token
						++index_.column;
						break;
					case token_id::new_line://Update markers
						update_markers_();
						break;
					default:
						++index_.column;
						break;
					}
					
					if (info.formatter != nullptr){//Pass to formatter
						generic_token_formatter::match_info formatter_info{
							index,
							matched_value,
							match_info.index
						};

						cache_.branch();
						if ((value = info.formatter->format(formatter_info, info)) != nullptr)
							id = info.rule.map_index(value->get_match_index());

						cache_.unbranch();
					}

					if (cache){//Cache
						if (value == nullptr)//Create value
							cache_.append(value = std::make_shared<token>(index, matched_value, match_info.index));
						else//Use value
							cache_.append(value);
					}

					if (info.skipper != nullptr && info.skipper->is(id))//Skip value
						return next_(info, count, cache);

					return (value == nullptr) ? std::make_shared<token>(index, matched_value, match_info.index) : value;
				}

				virtual void update_markers_(){
					++index_.line;
					index_.column = 1;
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
