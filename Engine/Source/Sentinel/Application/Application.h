#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Strings/StringView.h"

int main(int argc, char** argv);

namespace Sentinel {
    class Window;

    class Application {
    public:
        Application(CChar* name = "Application");
        virtual ~Application() = default;

        static Application& Get() { return *s_Instance; }

        inline Window* GetWindow() { return m_Window; }

    protected:
        virtual void Run() = 0;

    protected:
        Window* m_Window;
        StringView m_Title;

    private:
        static Application* s_Instance;

    private:
        friend int ::main(int argc, char** argv);
    };

    // TO BE Defined in Client
    Application* CreateApplication();
}  // namespace Sentinel
