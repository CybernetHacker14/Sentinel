#include "Scribe.h"

#include "Window/ScribeWindow.h"

#include "Renderer/ScribeRenderer.h"
#include "Renderer/ScribeImGuiBase.h"

#include "Panels/SceneHierarchyPanel.h"

#include <Sentinel/Application/EntryPoint.h>
#include <Sentinel/GUI/ImGui/ImGuiLayer.h>

#include <Sentinel/ECS/Scene.h>
#include <Sentinel/ECS/SceneManager.h>

#include <Sentinel/Archive/ZipFileOperations.h>

// For launching the application with Nvidia card if available by default
extern "C" {
__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Scribe {
    Scribe::Scribe() {
        m_CloseIndex = SubscribeToEvent(Sentinel::EventType::WindowClose, ST_BIND_FN(OnWindowClose));
        m_RunFunction = ST_BIND_FN(Run);

        Sentinel::WindowProperties props;
        props.Title = "Scribe";
        props.Width = 1920;
        props.Height = 1080;
        props.Mode = Sentinel::WindowMode::BORDERLESSMAXIMIZED;
        props.FramebufferTransparency = false;

        m_Window = new Window::ScribeWindow(props);
        m_Window->SetEventCallback(ST_BIND_FN(RaiseEvent));

        m_BaseRenderer = new Rendering::ScribeRenderer(m_Window);
        m_ImGuiLayer = new Sentinel::ImGuiLayer(m_BaseRenderer->GetRenderingContext());
        m_ImGuiBase = new Rendering::ScribeImGuiBase(
            m_BaseRenderer->GetRenderingContext(), static_cast<Window::ScribeWindow*>(m_Window));

        m_BaseRenderer->OnAttach();
        m_ImGuiLayer->OnAttach();
        m_ImGuiBase->OnAttach();

        m_TestScene = new Sentinel::Scene();
        if (Sentinel::Filesystem::DoesFileExist("Test.pak")) {
            char* buffer;
            if (Sentinel::ZipFileOperations::ReadFromZipFile("Test.pak", "Scenes/Test.scene", (void**)&buffer)) {
                std::stringstream stream(std::ios::in | std::ios::out);
                for (int i = 0; i < 118; i++) stream << buffer[i];
                free(buffer);
                m_TestScene->DeserializeFromStream(stream);

            } else {
                m_TestScene->SetName("Untitled_Scene_001");
            }
            // m_TestScene->DeserializeFromFile("Test.scene");
        } else {
            m_TestScene->SetName("Untitled_Scene_001");
        }

        ST_INFO("{0}", m_TestScene->GetUUID().ToString().c_str());
        m_Entity1 = m_TestScene->CreateEntity("Entity 1");
        m_Entity2 = m_TestScene->CreateEntity("Entity 2");
        m_Entity3 = m_TestScene->CreateEntity("Entity 3");
        m_Entity4 = m_TestScene->CreateEntity("Entity 4");
        m_Entity5 = m_TestScene->CreateEntity("Entity 5");
        m_Entity6 = m_TestScene->CreateEntity("Entity 6");
        m_Entity7 = m_TestScene->CreateEntity("Entity 7");

        m_Entity2->SetParent(m_Entity1);
        m_Entity3->SetParent(m_Entity2);
        m_Entity4->SetParent(m_Entity1);
        m_Entity6->SetParent(m_Entity5);
        m_Entity7->SetParent(m_Entity5);

        m_ImGuiBase->GetSceneHierarchyPanel()->SetScene(m_TestScene);
        // m_TestScene->SerializeToFile("Test.scene");

        m_TestScene->SetName("Untitled_Scene_001");
        Sentinel::ZipFileOperations::WriteToZipFile("Test.pak", "Scenes/Test2.scene", m_TestScene->SerializeToStream());

        /*m_TestScene->SetName("Untitled_World_001");
        std::stringstream stream = m_TestScene->SerializeToStream("Scenes/Test.scene");
        stream.seekg(0, std::ios::end);
        Sentinel::UInt32 length = stream.tellg();
        stream.seekg(0, std::ios::beg);

        Sentinel::Filesystem::CreateZipFile("Test.pak", "Scenes/Test.scene", stream.str().c_str(), length);*/
    }

    Scribe::~Scribe() {
        UnsubscribeFromEvent(Sentinel::EventType::WindowClose, m_CloseIndex);
        delete m_TestScene;
        delete m_Window;
    }

    void Scribe::Run() {
        while (m_Running) {
            if (!m_Minimized) {
                m_BaseRenderer->OnRender();
                m_ImGuiLayer->Begin();
                m_ImGuiBase->OnImGuiRender();
                m_ImGuiLayer->End();
                m_ImGuiBase->OnPostRender();
            }
            m_Window->OnUpdate();
            Sentinel::Input::OnUpdate();
        }
        m_ImGuiBase->OnDetach();
        m_ImGuiLayer->OnDetach();
        m_BaseRenderer->OnDetach();
        m_Window->Shutdown();
    }

    void Scribe::OnWindowClose(Sentinel::Event& event) {
        Sentinel::WindowCloseEvent e = *(event.Cast<Sentinel::WindowCloseEvent>());
        m_Running = false;
    }

}  // namespace Scribe