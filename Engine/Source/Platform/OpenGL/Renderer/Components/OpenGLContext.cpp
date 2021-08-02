#include "stpch.h"
#include "Platform/OpenGL/Renderer/Components/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sentinel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle) {
		ST_ENGINE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		ST_ENGINE_ASSERT(status, "Failed to initialize Glad!");

		m_ContextInfo.Vendor = (unsigned char*)glGetString(GL_VENDOR);
		m_ContextInfo.Renderer = (unsigned char*)glGetString(GL_RENDERER);
		m_ContextInfo.API = (unsigned char*)"OpenGL";
		m_ContextInfo.Version = (unsigned char*)glGetString(GL_VERSION);

		ST_ENGINE_INFO("VENDOR   : {0}", m_ContextInfo.Vendor);
		ST_ENGINE_INFO("RENDERER : {0}", m_ContextInfo.Renderer);
		ST_ENGINE_INFO("API      : {0}", m_ContextInfo.API);
		ST_ENGINE_INFO("VERSION  : {0}", m_ContextInfo.Version);

		ST_ENGINE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
			"Sentinel requires atleast OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}
