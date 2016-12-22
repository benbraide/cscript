#pragma once

#ifndef CSCRIPT_LITERAL_PARSER_H
#define CSCRIPT_LITERAL_PARSER_H

#include "../generic_parser.h"
#include "../../node/literal_node.h"
#include "../../lexer/operator_symbol.h"
#include "../../lexer/token_id_compare.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class literal : public generic{
			public:
				enum class suffix{
					nil,
					uint,
					long_,
					ulong,
					llong,
					ullong,
					float_,
					ldouble,
					wchar,
				};

				virtual node_type parse() override;

			private:
				virtual node_type parse_dec_();

				virtual node_type parse_hex_();

				virtual node_type parse_oct_();

				virtual node_type parse_rad_();

				virtual node_type parse_bin_();

				virtual node_type parse_real_();

				virtual node_type parse_single_(bool escaped);

				virtual node_type parse_double_(bool escaped);

				virtual node_type parse_back_(bool escaped);

				virtual suffix get_integral_suffix_(std::string &value);

				virtual suffix get_real_suffix_(std::string &value);

				virtual suffix get_string_suffix_(std::string &value);

				virtual int get_match_index_(boost::match_results<std::string::const_iterator> &results);
			};
		}
	}
}

#endif /* !CSCRIPT_LITERAL_PARSER_H */
