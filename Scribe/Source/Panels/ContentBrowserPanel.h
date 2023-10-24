#pragma once

#include <Sentinel.h>
#include <Sentinel/Filesystem/Filesystem.h>

namespace Scribe {
    namespace Panel {
        class ContentBrowserPanel {
        public:
            ContentBrowserPanel();

            void Display();

        private:
            void DrawLeftProjectPanel();
            void DrawLeftNodeTreeRecursive(const Sentinel::Path& path);

        private:
            Sentinel::Path m_BaseDirectory {"Assets"};
            Sentinel::Path m_CurrentSelectedPath {"Assets"};

            Sentinel::Float m_LeftPanelCurrentColumnWidth = 200.0f;
        };
    }  // namespace Panel
}  // namespace Scribe