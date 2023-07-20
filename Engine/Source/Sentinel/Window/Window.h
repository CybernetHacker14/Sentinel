#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Strings/SmallString.h"

namespace Sentinel {
    enum class WindowMode : UInt8 {
        WINDOWED = 0,
        WINDOWEDMAXIMIZED = 1,
        BORDERLESS = 2,
        BORDERLESSMAXIMIZED = 3,
        FULLSCREEN = 4
    };

    struct WindowProperties {
        SmallString Title;
        UInt16 Width;
        UInt16 Height;
        WindowMode Mode;
        Bool Transparent;

        WindowProperties(
            const SmallString& title = "Sentinel",
            UInt16 width = 900,
            UInt16 height = 900,
            WindowMode mode = WindowMode::WINDOWED,
            Bool transparency = false)
            : Title(title), Width(width), Height(height), Mode(mode), Transparent(transparency) {}
    };

    // The Window class needs to have atleast 1 leaf depth inheritance
    // This is to add/permit variations between applications. Variations can be
    // 1. The engine lib supplied default runtime
    // 2. Maybe a custom window class developed specifically for the attached application
    // 3. A definite custom window class developed for the editor

    // Let's give the CRTP thing a try again (Jesus not that path again)

    template<typename Derived_Window>
    class Window {
    public:
        inline Window(const Sentinel::WindowProperties& props) { m_WindowObject = Derived_Window(props); }

        virtual ~Window() = default;

        inline void Init() const { m_WindowObject.Init(); }
        inline void OnUpdate() const { m_WindowObject.OnUpdate(); }
        inline void SetVSync(Bool enabled) const { m_WindowObject.SetVSync(enabled); }
        inline void Shutdown() const { m_WindowObject.Shutdown(); }

        Derived_Window& GetNativeWindowVPtr() const { return m_WindowObject; }

        inline const UInt16 GetWidth() const { return m_Data.Width; }
        inline const UInt16 GetHeight() const { return m_Data.Height; }

        inline const Bool IsVSync() const { return m_Data.VSync; };

    protected:
        inline Window(const WindowProperties& props) : m_Properties(props) {}

    protected:
        // Basically using glfwSetWindowUserPointer we are setting a pointer to m_Data (line 148)
        // In the glfwSetXXXXCallback functions, we are retrieving the pointer to m_Data, then
        // setting the value of the EventCallbackFn.
        // In earlier days, when I was just learning C++, had no idea how this worked.
        // Now I need to sort this properly, because shit's getting undebuggable

        struct WindowData {
            SmallString Title;
            UInt16 Width, Height;
            Bool VSync;
        };

        WindowData m_Data;
        WindowProperties m_Properties;
        Derived_Window m_WindowObject;
    };
}  // namespace Sentinel
