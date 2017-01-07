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
			pointer,
			function,
		};

		template <id>
		struct id_type;

		template <>
		struct id_type<id::wchar>{
			typedef wchar_t value;
		};

		template <>
		struct id_type<id::char_>{
			typedef char value;
		};

		template <>
		struct id_type<id::uchar>{
			typedef unsigned char value;
		};

		template <>
		struct id_type<id::short_>{
			typedef short value;
		};

		template <>
		struct id_type<id::ushort>{
			typedef unsigned short value;
		};

		template <>
		struct id_type<id::int_>{
			typedef int value;
		};

		template <>
		struct id_type<id::uint>{
			typedef unsigned int value;
		};

		template <>
		struct id_type<id::long_>{
			typedef long value;
		};

		template <>
		struct id_type<id::ulong>{
			typedef unsigned long value;
		};

		template <>
		struct id_type<id::llong>{
			typedef long long value;
		};

		template <>
		struct id_type<id::ullong>{
			typedef unsigned long long value;
		};

		template <>
		struct id_type<id::float_>{
			typedef float value;
		};

		template <>
		struct id_type<id::double_>{
			typedef double value;
		};

		template <>
		struct id_type<id::ldouble>{
			typedef long double value;
		};
	}
}

#endif /* !CSCRIPT_TYPE_ID_H */
