/**
 *  @file oglplus/opt/debug_output.hpp
 *  @brief Wrapper for the GL debug output functionality
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_OPT_DEBUG_OUTPUT_1209031534_HPP
#define OGLPLUS_OPT_DEBUG_OUTPUT_1209031534_HPP

#include <oglplus/config.hpp>
#include <oglplus/string.hpp>
#include <oglplus/glfunc.hpp>
#include <oglplus/object.hpp>
#include <oglplus/exposed.hpp>
#include <oglplus/enumerations.hpp>

#include <cassert>
#include <stack>
#include <functional>
#include <memory>

namespace oglplus {

/// Debug output severity enumeration
/**
 *  @ingroup enumerations
 */
OGLPLUS_ENUM_CLASS_BEGIN(DebugOutputSeverity, GLenum)
#include <oglplus/enums/debug_output_severity.ipp>
OGLPLUS_ENUM_CLASS_END

#if !OGLPLUS_NO_ENUM_VALUE_NAMES
#include <oglplus/enums/debug_output_severity_names.ipp>
#endif

#if !OGLPLUS_ENUM_VALUE_RANGES
#include <oglplus/enums/debug_output_severity_range.ipp>
#endif


/// Debug output source enumeration
/**
 *  @ingroup enumerations
 */
OGLPLUS_ENUM_CLASS_BEGIN(DebugOutputSource, GLenum)
#include <oglplus/enums/debug_output_source.ipp>
OGLPLUS_ENUM_CLASS_END

#if !OGLPLUS_NO_ENUM_VALUE_NAMES
#include <oglplus/enums/debug_output_source_names.ipp>
#endif

#if !OGLPLUS_ENUM_VALUE_RANGES
#include <oglplus/enums/debug_output_source_range.ipp>
#endif

/// Debug output type enumeration
/**
 *  @ingroup enumerations
 */
OGLPLUS_ENUM_CLASS_BEGIN(DebugOutputType, GLenum)
#include <oglplus/enums/debug_output_type.ipp>
OGLPLUS_ENUM_CLASS_END

#if !OGLPLUS_NO_ENUM_VALUE_NAMES
#include <oglplus/enums/debug_output_type_names.ipp>
#endif

#if !OGLPLUS_ENUM_VALUE_RANGES
#include <oglplus/enums/debug_output_type_range.ipp>
#endif


#if OGLPLUS_DOCUMENTATION_ONLY || GL_VERSION_4_3

/// Wrapper for the GL debug output functionality
/**
 */
class Debug
{
public:
	/// Debug output severity
	typedef DebugOutputSeverity Severity;

	/// Debug output source
	typedef DebugOutputSource Source;

	/// Debug output type
	typedef DebugOutputType Type;


	/// Enables/disables messages with specific parameters
	/**
	 *  @glsymbols
	 *  @glfunref{DebugMessageControl}
	 */
	static void MessageControl(
		DebugOutputSource source,
		DebugOutputType type,
		DebugOutputSeverity severity,
		bool enable
	)
	{
		OGLPLUS_GLFUNC(DebugMessageControl)(
			GLenum(source),
			GLenum(type),
			GLenum(severity),
			0, nullptr,
			enable ? GL_TRUE : GL_FALSE
		);
		OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(DebugMessageControl));
	}

	/// Structure containing data passed to Callback functor
	struct CallbackData
	{
		DebugOutputSource source;
		DebugOutputType type;
		GLuint id;
		DebugOutputSeverity severity;
		GLsizei length;
		const GLchar* message;
	};

	/// Type of a callback functor processing debug output
	typedef std::function<void (const CallbackData&)> Callback;

	/// Installs a custom callback processing the debug output
	/**
	 *  Instances of this class install a new Callback function
	 *  processing the debug output messages in the constructor
	 *  and restoring the previous callback when destroyed.
	 *
	 *  @glsymbols
	 *  @glfunref{DebugMessageCallback}
	 *  @glfunref{GetPointerv}
	 *  @gldefref{DEBUG_CALLBACK_FUNCTION}
	 *  @gldefref{DEBUG_CALLBACK_USER_PARAM}
	 */
	class LogSink
	{
	private:
		static void GLAPIENTRY _gl_debug_proc(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			GLvoid* user_param
		)
		{
			LogSink* self = static_cast<LogSink*>(user_param);
			assert(self);
			if(self->_callback)
			{
				CallbackData data;
				data.source = DebugOutputSource(source);
				data.type = DebugOutputType(type);
				data.id = id;
				data.severity = DebugOutputSeverity(severity);
				data.length = length;
				data.message = message;
				self->_callback(data);
			}
		}

		Callback _callback;
		GLDEBUGPROC _prev_callback;
		void* _prev_context;
	public:
		/// Installs the @p callback and remembers the previous
		/**
		 *  @glsymbols
		 *  @glfunref{DebugMessageCallback}
		 */
		LogSink(Callback callback)
		 : _callback(callback)
		 , _prev_callback(nullptr)
		 , _prev_context(nullptr)
		{
			// get the previous callback
			GLDEBUGPROC _tmp_callback = nullptr;
			void** _tmp_ptr=reinterpret_cast<void**>(&_tmp_callback);
			OGLPLUS_GLFUNC(GetPointerv)(
				GL_DEBUG_CALLBACK_FUNCTION,
				_tmp_ptr
			);
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(GetPointerv));
			_prev_callback = _tmp_callback;

			//get the previous context
			OGLPLUS_GLFUNC(GetPointerv)(
				GL_DEBUG_CALLBACK_USER_PARAM,
				&_prev_context
			);
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(GetPointerv));

			OGLPLUS_GLFUNC(DebugMessageCallback)(
				&LogSink::_gl_debug_proc,
				static_cast<void*>(this)
			);
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(DebugMessageCallback));
		}

#if !OGLPLUS_NO_DELETED_FUNCTIONS
		/// LogSinks are not copyable
		LogSink(const LogSink&) = delete;
#else
	private:
		LogSink(const LogSink&);
	public:
#endif

		/// Restores the previous callback and its context
		/**
		 *  @glsymbols
		 *  @glfunref{DebugMessageCallback}
		 */
		~LogSink(void)
		{
			OGLPLUS_GLFUNC(DebugMessageCallback)(
				_prev_callback,
				_prev_context
			);
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(DebugMessageCallback));
		}
	};

	/// Pushes a group when constructed, pops in when destroyed
	class Group
	{
	public:
		/// Pushes a debug group with the specified parameters
		/**
		 *  @glsymbols
		 *  @glfunref{PushDebugGroup}
		 */
		Group(
			DebugOutputSource source,
			GLuint id,
			const GLchar* message,
			GLint length = -1
		)
		{
			OGLPLUS_GLFUNC(PushDebugGroup)(
				GLenum(source),
				id,
				length,
				message
			);
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(PushDebugGroup));
		}

		/// Pushes a debug group with the specified parameters
		/**
		 *  @overload
		 *
		 *  @glsymbols
		 *  @glfunref{PushDebugGroup}
		 */
		Group(
			DebugOutputSource source,
			GLuint id,
			const StrLit& message
		)
		{
			OGLPLUS_GLFUNC(PushDebugGroup)(
				GLenum(source),
				id,
				message.size(),
				message.c_str()
			);
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(PushDebugGroup));
		}

		/// Pushes a debug group with the specified parameters
		/**
		 *  @overload
		 *
		 *  @glsymbols
		 *  @glfunref{PushDebugGroup}
		 */
		Group(
			DebugOutputSource source,
			GLuint id,
			const String& message
		)
		{
			OGLPLUS_GLFUNC(PushDebugGroup)(
				GLenum(source),
				id,
				message.size(),
				message.c_str()
			);
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(PushDebugGroup));
		}

#if !OGLPLUS_NO_DELETED_FUNCTIONS
		/// Groups are not copyable
		Group(const Group&) = delete;
#else
	private:
		Group(const Group&);
	public:
#endif
		/// Pops a debug group with the specified parameters
		/**
		 *  @glsymbols
		 *  @glfunref{PopDebugGroup}
		 */
		~Group(void)
		{
			OGLPLUS_GLFUNC(PopDebugGroup)();
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(PopDebugGroup));
		}
	};

	/// Annotate @p object with the @p label with the specified @p length
	/**
	 *  @glsymbols
	 *  @glfunref{ObjectLabel}
	 */
	template <typename ObjectOps>
	static void ObjectLabel(
		const Object<ObjectOps>& object,
		const GLchar* label,
		GLint length = -1
	)
	{
		OGLPLUS_GLFUNC(ObjectLabel)(
			Expose(object).Name(),
			object.ObjectType(),
			length,
			label
		);
		OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(ObjectLabel));
	}

	/// Annotate @p object with the @p label
	/**
	 *  @overload
	 *
	 *  @glsymbols
	 *  @glfunref{ObjectLabel}
	 */
	template <typename ObjectOps>
	static void ObjectLabel(
		const Object<ObjectOps>& object,
		const StrLit& label
	)
	{
		OGLPLUS_GLFUNC(ObjectLabel)(
			Expose(object).Name(),
			object.ObjectType(),
			label.size(),
			label.c_str()
		);
		OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(ObjectLabel));
	}

	/// Annotate @p object with the @p label
	/**
	 *  @overload
	 *
	 *  @glsymbols
	 *  @glfunref{ObjectLabel}
	 */
	template <typename ObjectOps>
	static void ObjectLabel(
		const Object<ObjectOps>& object,
		const String& label
	)
	{
		OGLPLUS_GLFUNC(ObjectLabel)(
			Expose(object).Name(),
			object.ObjectType(),
			label.size(),
			label.c_str()
		);
		OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(ObjectLabel));
	}

	/// Enables or disables synchronous debug output
	/**
	 *  @glsymbols
	 *  @glfunref{Enable}
	 *  @glfunref{Disable}
	 *  @gldefref{DEBUG_OUTPUT_SYNCHRONOUS}
	 */
	static void Synchronous(bool enable)
	{
		if(enable)
		{
			OGLPLUS_GLFUNC(Enable)(
				GL_DEBUG_OUTPUT_SYNCHRONOUS
			);
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(Enable));
		}
		else
		{
			OGLPLUS_GLFUNC(Disable)(
				GL_DEBUG_OUTPUT_SYNCHRONOUS
			);
			OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(Disable));
		}
	}

	/// Inserts a new message into the debug output
	/**
	 *  @glsymbols
	 *  @glfunref{DebugMessageInsert}
	 */
	static void InsertMessage(
		DebugOutputSource source,
		DebugOutputType type,
		GLuint id,
		DebugOutputSeverity severity,
		const GLchar* buffer,
		GLint length = -1
	)
	{
		OGLPLUS_GLFUNC(DebugMessageInsert)(
			GLenum(source),
			GLenum(type),
			id,
			GLenum(severity),
			length,
			buffer
		);
		OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(DebugMessageInsert));
	}

	/// Inserts a new message into the debug output
	/**
	 *  @overload
	 *
	 *  @glsymbols
	 *  @glfunref{DebugMessageInsert}
	 */
	static void InsertMessage(
		DebugOutputSource source,
		DebugOutputType type,
		GLuint id,
		DebugOutputSeverity severity,
		const StrLit& message
	)
	{
		OGLPLUS_GLFUNC(DebugMessageInsert)(
			GLenum(source),
			GLenum(type),
			id,
			GLenum(severity),
			message.size(),
			message.c_str()
		);
		OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(DebugMessageInsert));
	}

	/// Inserts a new message into the debug output
	/**
	 *  @overload
	 *
	 *  @glsymbols
	 *  @glfunref{DebugMessageInsert}
	 */
	static void InsertMessage(
		DebugOutputSource source,
		DebugOutputType type,
		GLuint id,
		DebugOutputSeverity severity,
		const String& message
	)
	{
		OGLPLUS_GLFUNC(DebugMessageInsert)(
			GLenum(source),
			GLenum(type),
			id,
			GLenum(severity),
			message.size(),
			message.c_str()
		);
		OGLPLUS_VERIFY(OGLPLUS_ERROR_INFO(DebugMessageInsert));
	}
};
#endif

} // namespace oglplus

#endif // include guard
