#include "stpch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Sentinel/Events/Categories/WindowEvent.h"
#include "Sentinel/Events/Categories/MouseEvent.h"
#include "Sentinel/Events/Categories/KeyEvent.h"

#include "Sentinel/Renderer/Interface/RendererAPI.h"

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

		/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);*/

		if (props.FullScreen)
		{
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
		}
		else
		{
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			if (props.WindowedMaximized)
			{
				glfwMaximizeWindow(m_Window);
			}
		}
		++s_GLFWWindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					Scope<Event> event(new KeyPressedEvent(key, 0));
					data.EventCallback(STL::move(event));
					break;
				}

				case GLFW_RELEASE:
				{
					Scope<Event> event(new KeyReleasedEvent(key));
					data.EventCallback(STL::move(event));
					break;
				}

				case GLFW_REPEAT:
				{
					Scope<Event> event(new KeyPressedEvent(key, 1));
					data.EventCallback(STL::move(event));
					break;
				}
			}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			Scope<Event> event(new KeyTypedEvent(keycode));
			data.EventCallback(STL::move(event));
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					Scope<Event> event(new MouseButtonPressedEvent(button));
					data.EventCallback(STL::move(event));
					break;
				}
				case GLFW_RELEASE:
				{
					Scope<Event> event(new MouseButtonReleasedEvent(button));
					data.EventCallback(STL::move(event));
					break;
				}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			Scope<Event> event(new MouseScrolledEvent(xOffset, yOffset));
			data.EventCallback(STL::move(event));
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			Scope<Event> event(new MouseMovedEvent(xPos, yPos));
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
