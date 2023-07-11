#pragma once
#include "../global/base.h"

namespace ff
{
	namespace gl
	{
		static auto checkError() -> void
		{
			GLenum errorCode;
			bool needsAssert = false;
			std::string error;
			while ((errorCode = glGetError()) != GL_NO_ERROR)
			{
				needsAssert = true;
				switch (errorCode)
				{
					case GL_INVALID_ENUM: error = "INVALID_ENUM";
						break;
					case GL_INVALID_VALUE: error = "INVALID_VALUE";
						break;
					case GL_INVALID_OPERATION: error = "INVALID_OPERATION";
						break;
					case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW";
						break;
					case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW";
						break;
					case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY";
						break;
					case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION";
						break;
				}
				std::cout << error << std::endl;
			}
			assert(!needsAssert);
		}

		/// state
		static auto enable(GLenum cap) -> void
		{
			glEnable(cap);
			checkError();
		}

		static auto disable(GLenum cap) -> void
		{
			glDisable(cap);
			checkError();
		}

		static auto clearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) -> void
		{
			glClearColor(r, g, b, a);
			checkError();
		}

		static auto uniform1iv(GLint location, GLsizei size, GLint* value) -> void
		{
			glUniform1iv(location, size, value);
			checkError();
		}

		static auto uniform1i(GLint location, GLint value) -> void
		{
			glUniform1i(location, value);
			checkError();
		}


		/// blend
		static auto blendFuncSeparate(
			GLenum srcRGB,
			GLenum dstRGB,
			GLenum srcAlpha,
			GLenum dstAlpha) -> void
		{
			glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
			checkError();
		}

		static auto blendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) -> void
		{
			glBlendEquationSeparate(modeRGB, modeAlpha);
			checkError();
		}

		/// depth
		static auto depthMask(GLboolean flag) -> void
		{
			glDepthMask(flag);
			checkError();
		}

		static auto depthFunc(GLenum func) -> void
		{
			glDepthFunc(func);
			checkError();
		}

		static auto clearDepth(GLdouble depth) -> void
		{
			glClearDepth(depth);
			checkError();
		}
	}
}
