#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Events/Event.h"

namespace Sentinel {
    enum class WindowMode : UInt8 {
        WINDOWED = 0,
        WINDOWEDMAXIMIZED = 1,
        BORDERLESS = 2,
        BORDERLESSMAXIMIZED = 3,
        FULLSCREEN = 4
    };

    struct WindowProperties {
        STL::string Title;
        UInt32 Width;
        UInt32 Height;
        WindowMode Mode;
        Bool FramebufferTransparency;

        WindowProperties(
            const STL::string& title = "Sentinel-Engine",
            UInt32 width = 900,
            UInt32 height = 900,
            Sentinel::WindowMode mode = WindowMode::WINDOWED,
            Bool transparency = false)
            : Title(title), Width(width), Height(height), Mode(mode), FramebufferTransparency(transparency) {}
    };

    // Interface representing a desktop system based Window
    class Window {
    public:
        using EventCallbackFn = STL::delegate<void(UniqueRef<Event>)>;

        inline void Init() const {
            if (!m_InitFunction) return;

            m_InitFunction();
        }

        inline void OnUpdate() const {
            if (!m_OnUpdateFunction) return;

            m_OnUpdateFunction();
        }

        inline void SetVSync(Bool enabled) const {
            if (!m_SetVSyncFunction) return;

            m_SetVSyncFunction(enabled);
        }

        inline void Shutdown() const {
            if (!m_ShutdownFunction) return;

            m_ShutdownFunction();
        }

        inline const UInt32 GetWidth() const { return m_Data.Width; }
        inline const UInt32 GetHeight() const { return m_Data.Height; }

        inline const Bool IsVSync() const { return m_Data.VSync; };

        inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

    public:
        inline void* GetNativeWindowVPtr() const {
            if (!m_GetNativeWindowFunction) return nullptr;

            m_GetNativeWindowFunction();
        }

        template<typename T>
        inline T* GetNativeWindow() {
            return static_cast<T*>(GetNativeWindowVPtr());
        }

    public:
        static UniqueRef<Window> Create(const WindowProperties& props = WindowProperties());

    protected:
        inline Window(const WindowProperties& props) : m_Properties(props) {}

    protected:
        STL::delegate<void()> m_InitFunction;
        STL::delegate<void()> m_OnUpdateFunction;
        STL::delegate<void(Bool)> m_SetVSyncFunction;
        STL::delegate<void*()> m_GetNativeWindowFunction;
        STL::delegate<void()> m_ShutdownFunction;

    protected:
        struct WindowData {
            STL::string Title;
            UInt32 Width, Height;
            Bool VSync;

            EventCallbackFn EventCallback;
            EventCallbackFn InternalMouseMovedCallback;
            EventCallbackFn InternalWindowResizeCallback;
        };

        WindowData m_Data;
        WindowProperties m_Properties;
    };
}  // namespace Sentinel
