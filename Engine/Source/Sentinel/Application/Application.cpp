#include "stpch.h"
#include "Sentinel/Application/Application.h"

namespace Sentinel {
    Application* Application::s_Instance = nullptr;

    Application::Application(const STL::string& name) {
        ST_ENGINE_ASSERT(!s_Instance, "Application instance already exist!");
        s_Instance = this;
    }

    Application::~Application() {
    }

    const UInt32 Application::SubscribeToEvent(const EventType& eventType, const EventBus::EventCallbackFn& callback) {
        return m_EventBus.SubscribeToEvent(eventType, STL::move(callback));
    }

    void Application::UnsubscribeFromEvent(const EventType& eventType, const UInt32& callbackIndex) {
        m_EventBus.UnsubscribeFromEvent(eventType, callbackIndex);
    }

    void Application::RaiseEvent(UniqueRef<Event> eventData) {
        m_EventBus.NotifyAboutEvent(STL::move(eventData));
        m_EventBus.ProcessEvents();
    }

    void Application::Run() {
        m_RunFunction();
    }
}  // namespace Sentinel
