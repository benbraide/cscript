#pragma once

#ifndef CSCRIPT_PARSER_ID_H
#define CSCRIPT_PARSER_ID_H

#include "../common/preprocessor.h"

namespace cscript{
	namespace parser{
		enum class id : unsigned int{
			nil					= (0 << 0x0000),
			statement			= (1 << 0x0000),
			expression			= (1 << 0x0001),
			term				= (1 << 0x0001),
			literal				= (1 << 0x000),
			unary_operator		= (1 << 0x0002),
			binary_operator		= (1 << 0x0003),
		};

		/*
		LITERAL,
		KEYWORD,
		TERM,
		BOPERATOR,
		UOPERATOR,
		DECLARATION,
		DEPDECL,
		INITDECL,
		FUNCTION,
		GROUP,
		INDEX,
		BLOCK,
		EXPRESSION,
		STATEMENT,
		PREPROCESSOR,
		ATTRIBUTE,
		*/
	}
}

#endif /* !CSCRIPT_PARSER_ID_H */
