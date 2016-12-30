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
			collection,
			statement,
			block,
		};
	}
}

#endif /* !CSCRIPT_NODE_ID_H */
