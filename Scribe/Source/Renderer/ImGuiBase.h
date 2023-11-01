#pragma once

#define IM_VEC2_CLASS_EXTRA                                 \
    constexpr ImVec2(const glm::vec2& f) : x(f.x), y(f.y) { \
    }                                                       \
    operator glm::vec2() const {                            \
        return glm::vec2(x, y);                             \
    }

#include <Sentinel.h>
#include <Sentinel/Memory/FixedSlabAllocator.h>

namespace Sentinel {
    struct Texture2DData;
    struct ContextData;
}  // namespace Sentinel

namespace Scribe {
    namespace Window {
        class ScribeWindow;
    }

    namespace Panel {
        class SceneHierarchyPanel;
        class ContentBrowserPanel;
    }  // namespace Panel

    namespace Rendering {
        class ImGuiBase final {
        public:
            ImGuiBase(Sentinel::ContextData* context, Window::ScribeWindow* window);
            ~ImGuiBase();

            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnRender();
            void OnImGuiRender();
            void OnPostRender();

        private:
            void RenderImGuiTitleBar();
            void RenderImGuiTitleBar_RenderInfoPanel();

        private:
            Sentinel::FixedSlabAllocator<Sentinel::Texture2DData> m_TexMemAllocator;

            Sentinel::ContextData* m_Context = nullptr;
            Window::ScribeWindow* m_Window = nullptr;

            Sentinel::Texture2DData* m_TitlebarSpriteSheetTex = nullptr;
            Sentinel::Texture2DData* m_ContentBrowserSpriteSheetTex = nullptr;

            Panel::SceneHierarchyPanel* m_SceneHierarchyPanel = nullptr;
            Panel::ContentBrowserPanel* m_ContentBrowserPanel = nullptr;

        private:
            Sentinel::Float m_RenderInfoPanel_IconCellSize = 0.0f;
            Sentinel::Float m_RenderInfoPanel_LabelCellSize = 0.0f;
        };
    }  // namespace Rendering
}  // namespace Scribe