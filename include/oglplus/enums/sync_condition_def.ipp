/*
 *  .file oglplus/enums/sync_condition_def.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/sync_condition.txt' instead.
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifdef OGLPLUS_LIST_NEEDS_COMMA
# undef OGLPLUS_LIST_NEEDS_COMMA
#endif

#if defined GL_SYNC_GPU_COMMANDS_COMPLETE
# if OGLPLUS_LIST_NEEDS_COMMA
   OGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined GPUCommandsComplete
#  pragma push_macro("GPUCommandsComplete")
#  undef GPUCommandsComplete
   OGLPLUS_ENUM_CLASS_VALUE(GPUCommandsComplete, GL_SYNC_GPU_COMMANDS_COMPLETE)
#  pragma pop_macro("GPUCommandsComplete")
# else
   OGLPLUS_ENUM_CLASS_VALUE(GPUCommandsComplete, GL_SYNC_GPU_COMMANDS_COMPLETE)
# endif
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef OGLPLUS_LIST_NEEDS_COMMA
# undef OGLPLUS_LIST_NEEDS_COMMA
#endif

