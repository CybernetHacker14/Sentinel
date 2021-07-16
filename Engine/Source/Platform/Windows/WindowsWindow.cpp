#include "stpch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Sentinel/Events/Categories/WindowEvent.h"

#include "Sentinel/Renderer/Core/RendererAPI.h"

namespace Sentinel
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) {
		ST_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		ST_ENGINE_INFO("Creating window {0} : {1}, {2}", props.Title.c_str(), props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			int successs = glfwInit();
			ST_ENGINE_ASSERT(successs, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

	#if defined(ST_DEBUG)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	#endif // defined(ST_DEBUG)

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// GLFWCallbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			Scope<Event> event(new WindowResizeEvent(width, height));
			data.EventCallback(STL::move(event));
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			Scope<Event> event(new WindowCloseEvent());
			data.EventCallback(STL::move(event));
			});
	}
	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}
	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		m_Context->SwapBuffers();
	}
	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}
}
