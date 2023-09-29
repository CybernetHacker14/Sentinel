#pragma once

#include <Sentinel.h>

namespace Sentinel {
    struct ContextData;
}

namespace Scribe {
    namespace Panel {
        class RenderInfoPanel final {
        public:
            inline void SetRenderingContext(Sentinel::ContextData* context) { m_Context = context; }

            void DisplayInfoPanel();

        private:
            Sentinel::ContextData* m_Context;
        };
    }  // namespace Panel
}  // namespace Scribe