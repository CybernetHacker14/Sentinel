#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Events/EventBus.h"
#include "Sentinel/Window/Window.h"

int main(int argc, char** argv);

namespace Sentinel {
    class Application {
    public:
        Application(const STL::string& name = "Application");
        ~Application();

        static Application& Get() { return *s_Instance; }

        inline Window& GetWindow() { return *m_Window; }

    public:
        const UInt32 SubscribeToEvent(const EventType& eventType, const EventBus::EventCallbackFn& callback);
        void UnsubscribeFromEvent(const EventType& eventType, const UInt32& callback);

    protected:
        void Application::RaiseEvent(UniqueRef<Event> eventData);

        void Run();

    protected:
        Window* m_Window;
        EventBus m_EventBus;

    protected:
        STL::delegate<void()> m_RunFunction;

    private:
        static Application* s_Instance;

    private:
        friend int ::main(int argc, char** argv);
    };

    // TO BE Defined in Client
    Application* CreateApplication();
}  // namespace Sentinel
