#pragma once

#ifndef CSCRIPT_GENERIC_PARSER_H
#define CSCRIPT_GENERIC_PARSER_H

#include <list>
#include <memory>
#include <boost/filesystem.hpp>

#include "../node/generic_node.h"
#include "../lexer/generic_scanner.h"

#include "generic_context.h"

namespace cscript{
	namespace parser{
		struct parser_info;

		class generic{
		public:
			typedef lexer::generic_source::option source_option;

			typedef std::shared_ptr<node::generic> node_type;

			virtual ~generic(){}

			virtual node_type parse() = 0;
		};

		enum class state : unsigned int{
			nil				= (0 << 0x0000),
			typename_		= (1 << 0x0000),
			list			= (1 << 0x0001),
			loop			= (1 << 0x0002),
			function		= (1 << 0x0003),
			class_			= (1 << 0x0004),
			unary			= (1 << 0x0005),
		};

		struct path_info{
			boost::filesystem::path current;
			std::list<boost::filesystem::path> included_list;
		};

		struct parser_info{
			lexer::generic_source::token_type token;
			generic::node_type left_operand;
			generic_context *context;
			state states = state::nil;
			path_info path;
		};

		class save_left_operand{
		public:
			explicit save_left_operand(parser_info &info)
				: info_(info), left_operand_(info.left_operand){}

			~save_left_operand(){
				info_.left_operand = left_operand_;
			}

		private:
			parser_info &info_;
			generic::node_type left_operand_;
		};
		
		CSCRIPT_MAKE_OPERATORS(state)
	}
}

#endif /* !CSCRIPT_GENERIC_PARSER_H */
