#include "Scribe.h"

#include "Window/ScribeWindow.h"

#include "Renderer/EditorRenderer.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/ImGuiBase.h"

#include <Sentinel/Application/EntryPoint.h>
#include <Sentinel/GUI/ImGui/ImGuiLayer.h>

#include <Sentinel/ECS/Scene.h>

#include <Sentinel/Filesystem/Filesystem.h>
#include <Sentinel/Archive/ZipFileOperations.h>
#include <Sentinel/Resources/ImageResourceLoader.h>

#include "../Vendor/stb_image/stb_image.h"

// For launching the application with Nvidia card if available by default
extern "C" {
__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Scribe {
    Scribe::Scribe() {
        m_CloseIndex =
            Sentinel::EventsAPI::RegisterEvent(Sentinel::EventType::WindowClose, this, ST_BIND_FN(OnWindowClose));

        Sentinel::WindowProperties props;
        props.Title = "Scribe";
        props.Width = 1920;
        props.Height = 1080;
        props.Mode = Sentinel::WindowMode::BORDERLESSMAXIMIZED;
        props.Transparent = false;

        m_Window = new Window::ScribeWindow(props);

        m_BaseRenderer = new Rendering::EditorRenderer(m_Window);
        m_ImGuiLayer = new Sentinel::ImGuiLayer(m_BaseRenderer->GetContext());
        m_ImGuiBase =
            new Rendering::ImGuiBase(m_BaseRenderer->GetContext(), static_cast<Window::ScribeWindow*>(m_Window));

        m_SceneRenderer =
            new Rendering::SceneRenderer(m_Window, m_BaseRenderer->GetContext(), m_BaseRenderer->GetSwapchain());

        m_BaseRenderer->OnAttach();
        m_ImGuiLayer->OnAttach();
        m_ImGuiBase->OnAttach();

        m_SceneRenderer->OnAttach();

        m_ImGuiBase->SetViewportRenderer(m_SceneRenderer);

        /*
        Sentinel::Path archivePath("Test.pak");
        if (archivePath.DoesFileExist()) {
            Sentinel::UInt32 length;
            char* buffer;
            if (Sentinel::ZipFileOperations::ReadFromZipFile(
                    "Test.pak", "Scenes/Test.scene", (void**)&buffer, length)) {
                std::stringstream stream(std::ios::in | std::ios::out);
                for (int i = 0; i < length; i++) stream << buffer[i];
                delete buffer;
                m_TestScene->DeserializeFromStream(stream);

            } else {
                m_TestScene->SetName("Untitled_Scene_001");
            }
            // m_TestScene->DeserializeFromFile("Test.scene");
        } else {
            m_TestScene->SetName("Untitled_Scene_001");
        }
        */

        // m_ImGuiBase->GetSceneHierarchyPanel()->SetScene(m_TestScene);

        // m_TestScene->SetName("Untitled_World_001");
        // // TODO: Pass FileType, whether binary or non-binary
        /*Sentinel::ZipFileOperations::WriteBufferToZipFile(
            "Test.pak", "Scenes/Test.scene", m_TestScene->SerializeToStream());*/

        // int width, height, bpp;
        // auto data = stbi_load("Assets/Icons/grid.png", &width, &height, &bpp, 4);

        //// Sentinel::Filesystem::WriteToFileAtPath("Test.img", (Sentinel::UInt8*)&bpp, sizeof(int));
        // Sentinel::ImageResource* resource = new Sentinel::ImageResource();
        // resource->Channels = bpp;
        // resource->Width = width;
        // resource->Height = height;
        // resource->Pixels = data;

        // Sentinel::ImageResourceLoader::SaveToFile("Grid.sibf", resource);

        // delete resource;
    }

    Scribe::~Scribe() {
        Sentinel::EventsAPI::UnregisterEvent(Sentinel::EventType::WindowClose, m_CloseIndex);
        delete m_SceneRenderer;
        delete m_ImGuiBase;
        delete m_ImGuiLayer;
        delete m_BaseRenderer;
        delete m_Window;
    }

    void Scribe::Run() {
        while (m_Running) {
            if (!m_Minimized) {
                m_SceneRenderer->OnUpdate();
                m_SceneRenderer->OnRender();
                m_BaseRenderer->OnRender();
                m_ImGuiLayer->Begin();
                m_ImGuiBase->OnImGuiRender();
                m_ImGuiLayer->End();
                m_ImGuiBase->OnPostRender();
                m_SceneRenderer->OnPostRender();
                m_BaseRenderer->OnPostRender();
            }
            m_Window->OnUpdate();
            Sentinel::Input::OnUpdate();
        }
        m_SceneRenderer->OnDetach();
        m_ImGuiBase->OnDetach();
        m_ImGuiLayer->OnDetach();
        m_BaseRenderer->OnDetach();
        m_Window->Shutdown();
    }

    Sentinel::Bool Scribe::OnWindowClose(Sentinel::EventType type, Sentinel::EventData data, void* listener) {
        Scribe* app = (Scribe*)listener;
        app->m_Running = false;
        return true;
    }

}  // namespace Scribe