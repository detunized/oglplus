/**
 *  @file oglplus/shapes/draw.hpp
 *  @brief Implementation of shape draw instructions
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_SHAPES_DRAW_1107121519_HPP
#define OGLPLUS_SHAPES_DRAW_1107121519_HPP

namespace oglplus {
namespace shapes {

/// Structure containing information about how to draw a part of a shape
/**
 *  @note Do not use this class directly, use DrawingInstructions returned
 *  by the various shape builder classes instead.
 */
struct DrawOperation
{
	/// Enumeration of drawing methods
	enum class Method {
		DrawArrays,
		DrawElements
		// TODO
	};

	/// The method to be used to draw
	Method method;
	/// The primitive type to be used to draw
	PrimitiveType mode;

	/// The first element
	GLuint first;

	/// Count of elements
	GLuint count;

	/// Draw the part of a shape
	template <typename IT>
	void Draw(
		const std::vector<IT>& indices,
		GLuint inst_count = 1
	) const
	{
		if(inst_count == 1)
		{
			switch(method)
			{
				case Method::DrawArrays:
					return _DrawArrays();
				case Method::DrawElements:
					return _DrawElements(indices);
			}
		}
		else
		{
			switch(method)
			{
				case Method::DrawArrays:
					return _DrawArrays(inst_count);
				case Method::DrawElements:
					return _DrawElements(
						indices,
						inst_count
					);
			}
		}
	}
private:
	void _DrawArrays(void) const
	{
		::glDrawArrays(GLenum(mode), first, count);
		ThrowOnError(OGLPLUS_ERROR_INFO(DrawArrays));
	}

	void _DrawArrays(GLuint inst_count) const
	{
		::glDrawArraysInstanced(
			GLenum(mode),
			first,
			count,
			inst_count
		);
		ThrowOnError(OGLPLUS_ERROR_INFO(DrawArraysInstanced));
	}

	template <typename IT>
	void _DrawElements(const std::vector<IT>& indices) const
	{
		::glDrawElements(
			GLenum(mode),
			count,
			GLenum(GetDataType<IT>()),
			(void*)(indices.data() + first)
		);
		ThrowOnError(OGLPLUS_ERROR_INFO(DrawElements));
	}

	template <typename IT>
	void _DrawElements(
		const std::vector<IT>& indices,
		GLuint inst_count
	) const
	{
		::glDrawElementsInstanced(
			GLenum(mode),
			count,
			GLenum(GetDataType<IT>()),
			(void*)(indices.data() + first),
			inst_count
		);
		ThrowOnError(OGLPLUS_ERROR_INFO(DrawElementsInstanced));
	}
};

class DrawingInstructionWriter;

/// Class encapsulating the instructions for drawing of a shape
/** Instances of this class are produced by various shape builder
 *  classes implemented in the @c oglplus::shapes namespace and
 *  can be used to draw a shape from the data stored in vertex
 *  attrib buffers initialized from data returned by the member
 *  functions of the shape builder classes.
 *  @note Do not instantiate this class directly, use the appropriate
 *  member functions of the shape builder classes.
 *
 *  @see Cube
 */
class DrawingInstructions
{
private:

	typedef std::vector<DrawOperation> DrawOperationSeq;
	DrawOperationSeq _ops;

	DrawingInstructions(void)
	{ }

	DrawingInstructions(DrawOperationSeq&& ops)
	 : _ops(std::move(ops))
	{ }

	friend class DrawingInstructionWriter;
public:
	DrawingInstructions(DrawingInstructions&& temp)
	 : _ops(std::move(temp._ops))
	{ }

	DrawingInstructions(const DrawingInstructions& other)
	 : _ops(other._ops)
	{ }

	/// Draw the shape from data in currently bound VBOs indexed by indices
	template <typename IT>
	void Draw(const std::vector<IT>& indices, GLuint inst_count = 1) const
	{
		for(auto i=_ops.begin(),e=_ops.end();i!=e;++i)
			i->Draw(indices, inst_count);
	}
};

// Helper base class for shape builder classes making the drawing instructions
class DrawingInstructionWriter
{
private:
	typedef DrawingInstructions::DrawOperationSeq Operations;
protected:
	static DrawingInstructions MakeInstructions(void)
	{
		return DrawingInstructions();
	}

	static void AddInstruction(
		DrawingInstructions& instr,
		const DrawOperation& operation
	)
	{
		instr._ops.push_back(operation);
	}

	static DrawingInstructions MakeInstructions(Operations&& ops)
	{
		return DrawingInstructions(std::forward<Operations>(ops));
	}
};

} // shapes
} // oglplus

#endif // include guard