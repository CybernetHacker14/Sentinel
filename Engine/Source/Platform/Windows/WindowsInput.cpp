#include "stpch.h"
#include "Sentinel/Input/Input.h"
#include "Sentinel/Application/Application.h"

#include "GLFW/glfw3.h"

namespace Sentinel
{
	static STL::unordered_map<KeyCode, Bool> s_KeyStateMap(0);
	static STL::unordered_map<MouseCode, Bool> s_MouseButtonStateMap(0);

	const static STL::vector<KeyCode> s_AllKeys =
	{
		Key::Space,
		Key::Apostrophe,
		Key::Comma,
		Key::Minus,
		Key::Period,
		Key::Slash,
		Key::D0,
		Key::D1,
		Key::D2,
		Key::D3,
		Key::D4,
		Key::D5,
		Key::D6,
		Key::D7,
		Key::D8,
		Key::D9,
		Key::Semicolon,
		Key::Equal,
		Key::A,
		Key::B,
		Key::C,
		Key::D,
		Key::E,
		Key::F,
		Key::G,
		Key::H,
		Key::I,
		Key::J,
		Key::K,
		Key::L,
		Key::M,
		Key::N,
		Key::O,
		Key::P,
		Key::Q,
		Key::R,
		Key::S,
		Key::T,
		Key::U,
		Key::V,
		Key::W,
		Key::X,
		Key::Y,
		Key::Z,
		Key::LeftBracket,
		Key::Backslash,
		Key::RightBracket,
		Key::GraveAccent,
		Key::World1,
		Key::World2,
		Key::Escape,
		Key::Enter,
		Key::Tab,
		Key::Backspace,
		Key::Insert,
		Key::Delete,
		Key::Right,
		Key::Left,
		Key::Down,
		Key::Up,
		Key::PageUp,
		Key::PageDown,
		Key::Home,
		Key::End,
		Key::CapsLock,
		Key::ScrollLock,
		Key::NumLock,
		Key::PrintScreen,
		Key::Pause,
		Key::F1,
		Key::F2,
		Key::F3,
		Key::F4,
		Key::F5,
		Key::F6,
		Key::F7,
		Key::F8,
		Key::F9,
		Key::F10,
		Key::F11,
		Key::F12,
		Key::F13,
		Key::F14,
		Key::F15,
		Key::F16,
		Key::F17,
		Key::F18,
		Key::F19,
		Key::F20,
		Key::F21,
		Key::F22,
		Key::F23,
		Key::F24,
		Key::F25,
		Key::KP0,
		Key::KP1,
		Key::KP2,
		Key::KP3,
		Key::KP4,
		Key::KP5,
		Key::KP6,
		Key::KP7,
		Key::KP8,
		Key::KP9,
		Key::KPDecimal,
		Key::KPDivide,
		Key::KPMultiply,
		Key::KPSubtract,
		Key::KPAdd,
		Key::KPEnter,
		Key::KPEqual,
		Key::LeftShift,
		Key::LeftControl,
		Key::LeftAlt,
		Key::LeftSuper,
		Key::RightShift,
		Key::RightControl,
		Key::RightAlt,
		Key::RightSuper,
		Key::Menu
	};

	const static STL::vector<MouseCode> s_AllMouseButtons =
	{
		Mouse::Button0,
		Mouse::Button1,
		Mouse::Button2,
		Mouse::Button3,
		Mouse::Button4,
		Mouse::Button5,
		Mouse::Button6,
		Mouse::Button7
	};

	Bool Input::IsKeyPressed(const KeyCode keycode) {
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<Int32>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	Bool Input::IsKeyDown(const KeyCode keycode) {
		return GetKey(keycode) && !s_KeyStateMap[keycode];
	}

	Bool Input::IsKeyUp(const KeyCode keycode) {
		return !GetKey(keycode) && s_KeyStateMap[keycode];
	}

	Bool Input::IsMouseButtonPressed(const MouseCode button) {
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<Int32>(button));
		return state == GLFW_PRESS;
	}

	Bool Input::IsMouseButtonDown(const MouseCode button) {
		return GetMouseButton(button) && !s_MouseButtonStateMap[button];
	}

	Bool Input::IsMouseButtonUp(const MouseCode button) {
		return !GetMouseButton(button) && s_MouseButtonStateMap[button];
	}

	glm::vec2 Input::GetMousePosition() {
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (Float)xpos,(Float)ypos };
	}

	Float Input::GetMouseX() {
		return GetMousePosition().x;
	}

	Float Input::GetMouseY() {
		return GetMousePosition().y;
	}

	Bool Input::GetKey(const KeyCode keycode) {
		return glfwGetKey(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			static_cast<Int32>(keycode)) == GLFW_PRESS;
	}

	Bool Input::GetMouseButton(const MouseCode mousecode) {
		return glfwGetMouseButton(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			static_cast<Int32>(mousecode)) == GLFW_PRESS;
	}

	void Input::OnUpdate() {
		for (KeyCode key : s_AllKeys)
		{
			s_KeyStateMap[key] = GetKey(key);
		}

		for (MouseCode mouseButton : s_AllMouseButtons)
		{
			s_MouseButtonStateMap[mouseButton] = GetMouseButton(mouseButton);
		}
	}
}
