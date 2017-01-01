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
			declaration,
			type_cast,
			placeholder,
			operator_value_,
			array_type,
			pointer_type,
			function_type,
			collection,
			statement,
			block,
			identifier_compatible,
			type_compatible,
		};
	}
}

#endif /* !CSCRIPT_NODE_ID_H */
