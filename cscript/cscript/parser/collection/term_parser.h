#pragma once

#ifndef CSCRIPT_TERM_PARSER_H
#define CSCRIPT_TERM_PARSER_H

#include "../generic_parser.h"

#include "../../node/unary_operator_node.h"
#include "../../node/identifier_node.h"
#include "../../node/type_node.h"

namespace cscript{
	namespace parser{
		namespace collection{
			class term : public generic{
			public:
				virtual node_type parse() override;

			protected:
				virtual bool is_storage_class_(lexer::token_id id) const;

				virtual memory::virtual_address::attribute convert_storage_class_(lexer::token_id id) const;
			};
		}
	}
}

#endif /* !CSCRIPT_TERM_PARSER_H */
