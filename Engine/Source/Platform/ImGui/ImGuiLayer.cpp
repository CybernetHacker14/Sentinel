#include "stpch.h"
#include "Sentinel/GUI/ImGui/ImGuiLayer.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Events/Categories/WindowEvent.h"
#include "Sentinel/Graphics/Common/Backend.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_win32.h>

#include <GLFW/glfw3.h>

#include <backends/imgui_impl_dx11.h>

namespace Sentinel {
    ImGuiLayer::ImGuiLayer(ContextData* context) : Layer("ImGuiLayer"), m_Context(context) {
        m_AttachFunction = ST_BIND_EVENT_FN(ImGuiLayer::OnAttach);
        m_DetachFunction = ST_BIND_EVENT_FN(ImGuiLayer::OnDetach);
    }

    ImGuiLayer::~ImGuiLayer() {
    }

    void ImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        // io.ConfigFlags != ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform windows
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskbarIcons;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        float fontSize = 18.0f;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = app.GetWindow().GetNativeWindow<GLFWwindow>();
        ImGui_ImplGlfw_InitForOther(window, true);

        // Setup Platform/Renderer Bindings

        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(m_Context);
                auto device = DX11ContextAPI::GetDevice(context);
                auto nativeContext = DX11ContextAPI::GetNativeContext(context);
                ImGui_ImplDX11_Init(device, nativeContext);
                break;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); break;
        }

        m_OnResizeCallbackIndex =
            Application::Get().SubscribeToEvent(EventType::WindowResize, ST_BIND_EVENT_FN(OnResize));
    }

    void ImGuiLayer::OnDetach() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        Application::Get().UnsubscribeFromEvent(EventType::WindowResize, m_OnResizeCallbackIndex);
    }

    void ImGuiLayer::Begin() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        // Rendering
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnResize(Event& event) {
        WindowResizeEvent e = *(event.Cast<WindowResizeEvent>());
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
    }
}  // namespace Sentinel
