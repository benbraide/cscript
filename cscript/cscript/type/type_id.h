#pragma once

#ifndef CSCRIPT_TYPE_ID_H
#define CSCRIPT_TYPE_ID_H

namespace cscript{
	namespace type{
		enum class id{
			nil,
			auto_,
			any,
			void_,
			bool_,
			bit,
			byte,
			wchar,
			char_,
			uchar,
			short_,
			ushort,
			int_,
			uint,
			long_,
			ulong,
			llong,
			ullong,
			float_,
			double_,
			ldouble,
			string,
			array_,
		};
	}
}

#endif /* !CSCRIPT_TYPE_ID_H */
