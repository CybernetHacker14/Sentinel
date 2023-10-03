#pragma once

#include <Sentinel.h>

namespace Scribe {
    namespace Rendering {
        class SceneRenderer;
    }

    namespace Panel {
        class ViewportPanel final {
        public:
            inline void SetRenderer(Rendering::SceneRenderer* renderer) { m_Renderer = renderer; }

            void DisplayViewport();

        private:
            Rendering::SceneRenderer* m_Renderer;
        };
    }  // namespace Panel
}  // namespace Scribe