#include "Scribe.h"

#include <Sentinel/Application/EntryPoint.h>

#include "Window/EditorWindow.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// For launching the application with Nvidia card if available by default
extern "C" {
__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Scribe {
    Scribe::Scribe() {
        m_RunFunction = ST_BIND_EVENT_FN(Scribe::Run);

        Sentinel::WindowProperties props;
        props.Title = "Scribe";
        props.Width = 1280;
        props.Height = 720;
        props.Mode = Sentinel::WindowMode::BORDERLESS;
        props.FramebufferTransparency = false;

        m_Window = Sentinel::CreateUniqueRef<Window::EditorWindow>(props);
        m_Window->SetEventCallback(ST_BIND_EVENT_FN(RaiseEvent));
    }

    void Scribe::Run() {
        while (m_Running) {
            if (!m_Minimized) {}
            m_Window->OnUpdate();
            Sentinel::Input::OnUpdate();
        }
        m_Window->Shutdown();
    }
}  // namespace Scribe