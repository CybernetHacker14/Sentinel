#pragma once

#include <Sentinel.h>
#include <Sentinel/Filesystem/Filesystem.h>
#include <Sentinel/Memory/FixedSlabAllocator.h>

namespace Sentinel {
    struct Texture2DData;
}  // namespace Sentinel

namespace Scribe {
    namespace Rendering {
        class ImGuiBase;
    }

    namespace Panel {
        class ContentBrowserPanel {
        public:
            ContentBrowserPanel(Sentinel::Texture2DData* spriteSheet);
            ~ContentBrowserPanel();

            void Display();

        private:
            void DrawProjectPanel();
            void DrawProjectPanel_MenuBar();
            void DrawProjectPanel_MiddleSection();
            void DrawProjectPanel_RightClickContextMenu();

            void UpdateCurrentSelectedFolderCachedData();
            void ComputeIconLabelStrings();
            void GoToPreviousFolder();

        private:
            Sentinel::Path m_BaseDirectory {"Assets"};
            Sentinel::Path m_CurrentSelectedPath {"Assets"};

            Sentinel::Vector<Sentinel::Path> m_CurrentSelectedFolderImmediatePaths;
            Sentinel::Vector<Sentinel::String> m_CurrentSelectedFolderImmediatePathStrings;

            Sentinel::Vector<Sentinel::Path> m_CurrentSelectedFolderParentHierarchyPaths;

            Sentinel::Vector<Sentinel::Path> m_CurrentSelectedPathHierarchyStack;

            Sentinel::Float m_LeftPanelCurrentColumnWidth = 200.0f;

            Sentinel::UInt32 m_PopupWindowID;

        private:
            Sentinel::Texture2DData* m_SpriteSheetTex;
        };
    }  // namespace Panel
}  // namespace Scribe