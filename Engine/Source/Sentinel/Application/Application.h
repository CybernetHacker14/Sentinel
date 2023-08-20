#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Templates/Function.h"
#include "Sentinel/Common/Strings/StringView.h"
#include "Sentinel/Window/Window.h"

int main(int argc, char** argv);

namespace Sentinel {
    class Application {
    public:
        Application(CChar* name = "Application");
        virtual ~Application() = default;

        static Application& Get() { return *s_Instance; }

        inline Window& GetWindow() { return *m_Window; }

    protected:
        void Run();

    protected:
        Window* m_Window;
        StringView m_Title;

    protected:
        Function<void()> m_RunFunction;

    private:
        static Application* s_Instance;

    private:
        friend int ::main(int argc, char** argv);
    };

    // TO BE Defined in Client
    Application* CreateApplication();
}  // namespace Sentinel
