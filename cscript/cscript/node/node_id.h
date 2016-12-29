#pragma once

#ifndef CSCRIPT_NODE_ID_H
#define CSCRIPT_NODE_ID_H

namespace cscript{
	namespace node{
		enum class id{
			nil,
			literal,
			operator_,
			collection,
		};
	}
}

#endif /* !CSCRIPT_NODE_ID_H */
