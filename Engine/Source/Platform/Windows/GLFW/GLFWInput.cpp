#include "stpch.h"
#include "Sentinel/Input/Input.h"
#include "Sentinel/Window/Window.h"
#include "Sentinel/Application/Application.h"

#include <GLFW/glfw3.h>
#include <unordered_map>

namespace Sentinel {
    namespace GLFWInputUtils {
        static std::unordered_map<KeyCode, Bool> s_KeyStateMap(0);
        static std::unordered_map<MouseCode, Bool> s_MouseStateMap(0);

        const static KeyCode s_AllKeys[120] = {
            KeyCode::Space,
            KeyCode::Apostrophe,
            KeyCode::Comma,
            KeyCode::Minus,
            KeyCode::Period,
            KeyCode::Slash,
            KeyCode::D0,
            KeyCode::D1,
            KeyCode::D2,
            KeyCode::D3,
            KeyCode::D4,
            KeyCode::D5,
            KeyCode::D6,
            KeyCode::D7,
            KeyCode::D8,
            KeyCode::D9,
            KeyCode::Semicolon,
            KeyCode::Equal,
            KeyCode::A,
            KeyCode::B,
            KeyCode::C,
            KeyCode::D,
            KeyCode::E,
            KeyCode::F,
            KeyCode::G,
            KeyCode::H,
            KeyCode::I,
            KeyCode::J,
            KeyCode::K,
            KeyCode::L,
            KeyCode::M,
            KeyCode::N,
            KeyCode::O,
            KeyCode::P,
            KeyCode::Q,
            KeyCode::R,
            KeyCode::S,
            KeyCode::T,
            KeyCode::U,
            KeyCode::V,
            KeyCode::W,
            KeyCode::X,
            KeyCode::Y,
            KeyCode::Z,
            KeyCode::LeftBracket,
            KeyCode::Backslash,
            KeyCode::RightBracket,
            KeyCode::GraveAccent,
            KeyCode::World1,
            KeyCode::World2,
            KeyCode::Escape,
            KeyCode::Enter,
            KeyCode::Tab,
            KeyCode::Backspace,
            KeyCode::Insert,
            KeyCode::Delete,
            KeyCode::Right,
            KeyCode::Left,
            KeyCode::Down,
            KeyCode::Up,
            KeyCode::PageUp,
            KeyCode::PageDown,
            KeyCode::Home,
            KeyCode::End,
            KeyCode::CapsLock,
            KeyCode::ScrollLock,
            KeyCode::NumLock,
            KeyCode::PrintScreen,
            KeyCode::Pause,
            KeyCode::F1,
            KeyCode::F2,
            KeyCode::F3,
            KeyCode::F4,
            KeyCode::F5,
            KeyCode::F6,
            KeyCode::F7,
            KeyCode::F8,
            KeyCode::F9,
            KeyCode::F10,
            KeyCode::F11,
            KeyCode::F12,
            KeyCode::F13,
            KeyCode::F14,
            KeyCode::F15,
            KeyCode::F16,
            KeyCode::F17,
            KeyCode::F18,
            KeyCode::F19,
            KeyCode::F20,
            KeyCode::F21,
            KeyCode::F22,
            KeyCode::F23,
            KeyCode::F24,
            KeyCode::F25,
            KeyCode::KP0,
            KeyCode::KP1,
            KeyCode::KP2,
            KeyCode::KP3,
            KeyCode::KP4,
            KeyCode::KP5,
            KeyCode::KP6,
            KeyCode::KP7,
            KeyCode::KP8,
            KeyCode::KP9,
            KeyCode::KPDecimal,
            KeyCode::KPDivide,
            KeyCode::KPMultiply,
            KeyCode::KPSubtract,
            KeyCode::KPAdd,
            KeyCode::KPEnter,
            KeyCode::KPEqual,
            KeyCode::LeftShift,
            KeyCode::LeftControl,
            KeyCode::LeftAlt,
            KeyCode::LeftSuper,
            KeyCode::RightShift,
            KeyCode::RightControl,
            KeyCode::RightAlt,
            KeyCode::RightSuper,
            KeyCode::Menu};

        const static MouseCode s_AllMouseButtons[8] = {
            MouseCode::Button0,
            MouseCode::Button1,
            MouseCode::Button2,
            MouseCode::Button3,
            MouseCode::Button4,
            MouseCode::Button5,
            MouseCode::Button6,
            MouseCode::Button7};

        static Bool GetKey(KeyCode keycode) {
            GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNative());
            return glfwGetKey(window, static_cast<Int32>(keycode)) == GLFW_PRESS;
        }

        static Bool GetMouseButton(MouseCode mousecode) {
            return glfwGetMouseButton(
                       static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNative()),
                       static_cast<Int32>(mousecode)) == GLFW_PRESS;
        }

    }  // namespace GLFWInputUtils

    Bool Input::IsKeyPressed(const KeyCode keycode) {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNative());
        Int32 state = glfwGetKey(window, static_cast<Int32>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    Bool Input::IsKeyDown(const KeyCode keycode) {
        return GLFWInputUtils::GetKey(keycode) && !GLFWInputUtils::s_KeyStateMap[keycode];
    }

    Bool Input::IsKeyUp(const KeyCode keycode) {
        return !GLFWInputUtils::GetKey(keycode) && GLFWInputUtils::s_KeyStateMap[keycode];
    }

    Bool Input::IsMouseButtonPressed(const MouseCode button) {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNative());
        Int32 state = glfwGetMouseButton(window, static_cast<Int32>(button));
        return state == GLFW_PRESS;
    }

    Bool Input::IsMouseButtonDown(const MouseCode button) {
        return GLFWInputUtils::GetMouseButton(button) && !GLFWInputUtils::s_MouseStateMap[button];
    }

    Bool Input::IsMouseButtonUp(const MouseCode button) {
        return !GLFWInputUtils::GetMouseButton(button) && GLFWInputUtils::s_MouseStateMap[button];
    }

    glm::vec2 Input::GetMousePosition() {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNative());
        Double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {(Float)xpos, (Float)ypos};
    }

    Float Input::GetMouseX() {
        return GetMousePosition().x;
    }

    Float Input::GetMouseY() {
        return GetMousePosition().y;
    }

    // This polling should preferably run on a separate thread
    void Input::OnUpdate() {
        for (KeyCode key: GLFWInputUtils::s_AllKeys) {
            GLFWInputUtils::s_KeyStateMap[key] = GLFWInputUtils::GetKey(key);
        }

        for (MouseCode mouseButton: GLFWInputUtils::s_AllMouseButtons) {
            GLFWInputUtils::s_MouseStateMap[mouseButton] = GLFWInputUtils::GetMouseButton(mouseButton);
        }
    }
}  // namespace Sentinel
