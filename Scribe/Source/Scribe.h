#pragma once

#include <Sentinel.h>

namespace Scribe {
    class Scribe final: public Sentinel::Application {
    public:
        Scribe();

    private:
        void Run();

    private:
        Sentinel::Bool m_Running;
        Sentinel::Bool m_Minimized;
    };
}  // namespace Scribe

Sentinel::Application* Sentinel::CreateApplication() {
    return new Scribe::Scribe;
}