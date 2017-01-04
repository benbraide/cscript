#pragma once

#ifndef CSCRIPT_NODE_ID_H
#define CSCRIPT_NODE_ID_H

namespace cscript{
	namespace node{
		enum class id{
			nil,
			literal,
			operator_,
			identifier,
			type,
			type_with_storage,
			typedef_,
			declaration,
			type_cast,
			placeholder,
			decl_type,
			operator_value_,
			array_type,
			pointer_type,
			function_type,
			variadic_type,
			auto_type,
			collection,
			statement,
			block,
			identifier_compatible,
			type_compatible,
		};
	}
}

#endif /* !CSCRIPT_NODE_ID_H */
