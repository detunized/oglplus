/*
 *  .file oglplus/enums/ext/debug_output_source_range.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/ext/debug_output_source.txt' instead.
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

OGLPLUS_LIB_FUNC aux::CastIterRange<
	const GLenum*,
	DebugOutputARBSource
> EnumValueRange(DebugOutputARBSource*)
OGLPLUS_NOEXCEPT(true)
#if (!OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OGLPLUS_IMPL_EVR_DEBUGOUTPUTARBSOURCE)
#define OGLPLUS_IMPL_EVR_DEBUGOUTPUTARBSOURCE
{
static const GLenum _values[] = {
#if defined GL_DEBUG_SOURCE_API_ARB
GL_DEBUG_SOURCE_API_ARB,
#endif
#if defined GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB
GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB,
#endif
#if defined GL_DEBUG_SOURCE_SHADER_COMPILER_ARB
GL_DEBUG_SOURCE_SHADER_COMPILER_ARB,
#endif
#if defined GL_DEBUG_SOURCE_THIRD_PARTY_ARB
GL_DEBUG_SOURCE_THIRD_PARTY_ARB,
#endif
#if defined GL_DEBUG_SOURCE_APPLICATION_ARB
GL_DEBUG_SOURCE_APPLICATION_ARB,
#endif
#if defined GL_DEBUG_SOURCE_OTHER_ARB
GL_DEBUG_SOURCE_OTHER_ARB,
#endif
#if defined GL_DONT_CARE
GL_DONT_CARE,
#endif
0
};
return aux::CastIterRange<
	const GLenum*,
	DebugOutputARBSource
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif

