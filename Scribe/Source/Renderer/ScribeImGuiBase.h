#pragma once

#include <Sentinel.h>

namespace Scribe {
    namespace Rendering {
        class ScribeImGuiBase final: public Sentinel::Layer {
        public:
            ScribeImGuiBase();
            ~ScribeImGuiBase();

        private:
            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnRender();
            void OnImGuiRender();
            void OnPostRender();
        };
    }  // namespace Rendering
}  // namespace Scribe