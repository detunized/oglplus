/**
 *  @file oglplus/context/logical_ops.hpp
 *  @brief Wrappers for color logical operations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_CONTEXT_LOGICAL_OPS_1201040722_HPP
#define OGLPLUS_CONTEXT_LOGICAL_OPS_1201040722_HPP

#include <oglplus/config.hpp>
#include <oglplus/glfunc.hpp>
#include <oglplus/error.hpp>
#include <oglplus/color_log_op.hpp>

namespace oglplus {
namespace context {

/// Wrapper for the color buffer logical operations
/**
 *  @ingroup ogl_context
 */
class LogicalOps
{
public:
	/// Sets the color logical operation
	/**
	 *  @glsymbols
	 *  @glfunref{LogicOp}
	 */
	static void LogicOp(ColorLogicOperation op)
	{
		OGLPLUS_GLFUNC(LogicOp)(GLenum(op));
		OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(LogicOp));
	}

	/// Returns the color logical operation
	/**
	 *  @glsymbols
	 *  @glfunref{Get}
	 *  @gldefref{COLOR_LOGIC_OP}
	 */
	static ColorLogicOperation LogicOpMode(void)
	{
		GLint result;
		OGLPLUS_GLFUNC(GetIntegerv)(GL_LOGIC_OP_MODE, &result);
		OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(GetIntegerv));
		return ColorLogicOperation(result);
	}
};

} // namespace context
} // namespace oglplus

#endif // include guard
