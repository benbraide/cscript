#pragma once

#ifndef CSCRIPT_LITERAL_PARSER_H
#define CSCRIPT_LITERAL_PARSER_H

#include "../generic_parser.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class literal : public generic{
			public:
				virtual node_type parse(parser_info &info) override;

			private:
				virtual node_type parse_dec_(parser_info &info);

				virtual node_type parse_hex_(parser_info &info);

				virtual node_type parse_oct_(parser_info &info);

				virtual node_type parse_rad_(parser_info &info);

				virtual node_type parse_bin_(parser_info &info);

				virtual node_type parse_real_(parser_info &info);

				virtual node_type parse_exp_(parser_info &info);

				virtual node_type parse_single_(parser_info &info, bool escaped);

				virtual node_type parse_double_(parser_info &info, bool escaped);

				virtual node_type parse_back_(parser_info &info, bool escaped);
			};
		}
	}
}

#endif /* !CSCRIPT_LITERAL_PARSER_H */
