/*
 *  .file oglplus/enums/transform_feedback_mode.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/transform_feedback_mode.txt' instead.
 *
 *  Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#if OGLPLUS_DOCUMENTATION_ONLY

/// INTERLEAVED_ATTRIBS
InterleavedAttribs,
/// SEPARATE_ATTRIBS
SeparateAttribs

#else // OGLPLUS_DOCUMENTATION_ONLY

#ifdef OGLPLUS_LIST_NEEDS_COMMA
# undef OGLPLUS_LIST_NEEDS_COMMA
#endif

#if defined GL_INTERLEAVED_ATTRIBS
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
InterleavedAttribs = GL_INTERLEAVED_ATTRIBS
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined GL_SEPARATE_ATTRIBS
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
SeparateAttribs = GL_SEPARATE_ATTRIBS
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef OGLPLUS_LIST_NEEDS_COMMA
# undef OGLPLUS_LIST_NEEDS_COMMA
#endif

#endif // OGLPLUS_DOCUMENTATION_ONLY
