#include "stpch.h"

#ifdef ST_RENDERER_DX11

    #include "Sentinel/GUI/ImGui/ImGuiLayer.h"
    #include "Sentinel/Application/Application.h"

    #include "Sentinel/Events/Categories/WindowEvent.h"
    #include "Sentinel/Graphics/Common/Backend.h"

    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include <imgui.h>
    #include <backends/imgui_impl_glfw.h>

    #include <GLFW/glfw3.h>

    #include <backends/imgui_impl_dx11.h>

namespace Sentinel {
    ImGuiLayer::ImGuiLayer(ContextData* context) : m_Context(context) {
        Application::Get().SubscribeToEvent(EventType::WindowResize, ST_BIND_FN(OnResize));
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

        SetDarkThemeColors();

        Application& app = Application::Get();
        GLFWwindow* window = app.GetWindow().GetNativeWindow<GLFWwindow>();
        ImGui_ImplGlfw_InitForOther(window, true);

        // Setup Platform/Renderer Bindings

        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                auto device = ContextAPI::GetDevice(m_Context);
                auto nativeContext = ContextAPI::GetNativeContext(m_Context);
                ImGui_ImplDX11_Init(device, nativeContext);
                break;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); break;
        }

        m_OnResizeCallbackIndex = Application::Get().SubscribeToEvent(EventType::WindowResize, ST_BIND_FN(OnResize));
    }

    void ImGuiLayer::OnDetach() {
        Application::Get().UnsubscribeFromEvent(EventType::WindowResize, m_OnResizeCallbackIndex);
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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

    void ImGuiLayer::SetDarkThemeColors() {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4 {0.1f, 0.105f, 0.11f, 1.0f};

        // Headers
        colors[ImGuiCol_Header] = ImVec4 {0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_HeaderHovered] = ImVec4 {0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_HeaderActive] = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};

        // Buttons
        colors[ImGuiCol_Button] = ImVec4 {0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4 {0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ButtonActive] = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4 {0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = ImVec4 {0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_FrameBgActive] = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabHovered] = ImVec4 {0.38f, 0.3805f, 0.381f, 1.0f};
        colors[ImGuiCol_TabActive] = ImVec4 {0.28f, 0.2805f, 0.281f, 1.0f};
        colors[ImGuiCol_TabUnfocused] = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4 {0.2f, 0.205f, 0.21f, 1.0f};

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgActive] = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
    }
}  // namespace Sentinel

#endif  // ST_RENDERER_DX11
