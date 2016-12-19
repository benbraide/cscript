#pragma once

#ifndef CSCRIPT_PREPROCESSOR_H
#define CSCRIPT_PREPROCESSOR_H

#define CSCRIPT_MAKE_OPERATORS_(x, t)\
	inline constexpr x operator |(x l, x r){\
		return (x)((t)(l) | (t)(r));\
	}\
\
	inline constexpr x operator &(x l, x r){\
		return (x)((t)(l) & (t)(r));\
	}\
\
	inline constexpr x operator ~(x r){\
		return (x)(~(t)(r));\
	}\
\
	inline x operator |=(x &l, x r){\
		return (l = (x)((t)(l) | (t)(r)));\
	}\
\
	inline x operator &=(x &l, x r){\
		return (l = (x)((t)(l) & (t)(r)));\
	}\
\
	inline x operator <<(x &l, t r){\
		return (x)((t)(l) << (r));\
	}\
\
	inline x operator >>(x &l, t r){\
		return (x)((t)(l) >> (r));\
	}

#define CSCRIPT_MAKE_OPERATORS(x) CSCRIPT_MAKE_OPERATORS_(x, unsigned int)

#define CSCRIPT_MAKE_OPERATORS_EX(x) CSCRIPT_MAKE_OPERATORS_(x, unsigned long long)

#define CSCRIPT_SET(source, target) ((source) |= (target))
#define CSCRIPT_SET_V(source, target) ((source) | (target))

#define CSCRIPT_REMOVE(source, target) ((source) &= ~(target))
#define CSCRIPT_REMOVE_V(source, target) ((source) & ~(target))

#define CSCRIPT_IS(source, target) (((source) & (target)) == (target))
#define CSCRIPT_IS_ANY(source, target) (((source) & (target)) != static_cast<decltype(target)>(0))

#define CSCRIPT_CONVERT_ENUM_(x, t, v) (x)(t)(v)
#define CSCRIPT_CONVERT_ENUM(x, v) CSCRIPT_CONVERT_ENUM_(x, int, v)

#define CSCRIPT_EQUAL_ENUMS_(x, t, l, r) ((l) == (x)(t)(r))
#define CSCRIPT_EQUAL_ENUMS(x, l, r) CSCRIPT_EQUAL_ENUMS_(x, int, l, r)

#define CSCRIPT_INCREMENT_ENUM_(x, t, v) (v) = (x)((t)(v) + (t)1)
#define CSCRIPT_INCREMENT_ENUM(x, v) CSCRIPT_INCREMENT_ENUM_(x, int, v)

#define CSCRIPT_DECREMENT_ENUM_(x, t, v) (v) = (x)((t)(v) - (t)1)
#define CSCRIPT_DECREMENT_ENUM(x, v) CSCRIPT_DECREMENT_ENUM_(x, int, v)

#endif /* !CSCRIPT_PREPROCESSOR_H */
