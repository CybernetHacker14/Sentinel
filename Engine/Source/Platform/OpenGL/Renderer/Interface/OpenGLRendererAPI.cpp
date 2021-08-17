#include "stpch.h"
#include "Platform/OpenGL/Renderer/Interface/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Sentinel
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam) {
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH: ST_ENGINE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM: ST_ENGINE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW: ST_ENGINE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: ST_ENGINE_TRACE(message); return;
		}

		ST_ENGINE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init() {
	#ifdef ST_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif // ST_DEBUG

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(UInt x, UInt y, UInt width, UInt height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
