#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Strings/StringView.h"
#include "Sentinel/Common/Templates/Function.h"

namespace Sentinel {
    enum class WindowMode : UInt8 {
        WINDOWED = 0,
        WINDOWEDMAXIMIZED = 1,
        BORDERLESS = 2,
        BORDERLESSMAXIMIZED = 3,
        FULLSCREEN = 4
    };

    struct WindowProperties {
        StringView Title;
        UInt16 Width;
        UInt16 Height;
        WindowMode Mode;
        Bool Transparent;

        WindowProperties(
            CChar* title = "Sentinel",
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

    class Window {
    public:
        virtual ~Window() = default;

        inline void OnUpdate() const { m_UpdateFn(); }
        inline void SetVSync(Bool enabled) const { m_SetVSyncFn(enabled); }
        inline void Shutdown() const { m_ShutdownFn; }
        inline void* GetNative() const { return m_GetNativeFn(); }

        inline const UInt16 GetWidth() const { return m_Data.Width; }
        inline const UInt16 GetHeight() const { return m_Data.Height; }

    protected:
        inline Window(const WindowProperties& props) : m_Properties(props) {}

    protected:
        // Basically using glfwSetWindowUserPointer we are setting a pointer to m_Data (line 171)
        // In the glfwSetXXXXCallback functions, we are retrieving the pointer to m_Data, then
        // setting the value of the EventCallbackFn.
        // In earlier days, when I was just learning C++, had no idea how this worked.
        // Now I need to sort this properly, because shit's getting undebuggable

        Function<void()> m_UpdateFn;
        Function<void(Bool)> m_SetVSyncFn;
        Function<void()> m_ShutdownFn;
        Function<void*()> m_GetNativeFn;

        struct WindowData {
            UInt16 Width = 900, Height = 900;
            Bool VSync = true;
        };

        WindowData m_Data;
        WindowProperties m_Properties;
    };
}  // namespace Sentinel
