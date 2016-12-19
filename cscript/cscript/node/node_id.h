#pragma once

#ifndef CSCRIPT_NODE_ID_H
#define CSCRIPT_NODE_ID_H

namespace cscript{
	namespace node{
		enum class id : unsigned int{
			nil				= (0 << 0x0000),
		};
	}
}

#endif /* !CSCRIPT_NODE_ID_H */
