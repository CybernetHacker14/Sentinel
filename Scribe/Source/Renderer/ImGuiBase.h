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
        class RenderInfoPanel;
        class ViewportPanel;
    }  // namespace Panel

    namespace Rendering {
        class SceneRenderer;

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

            void SetViewportRenderer(Rendering::SceneRenderer* renderer);

        private:
            void RenderImGuiTitleBar();

        private:
            Sentinel::FixedSlabAllocator<Sentinel::Texture2DData> m_TexMemAllocator;

            Sentinel::ContextData* m_Context = nullptr;
            Window::ScribeWindow* m_Window = nullptr;

            Sentinel::Texture2DData* m_SpriteSheetTex = nullptr;

            Panel::SceneHierarchyPanel* m_SceneHierarchyPanel = nullptr;
            Panel::RenderInfoPanel* m_RenderInfoPanel = nullptr;
            Panel::ViewportPanel* m_ViewportPanel = nullptr;

        private:
            const float m_TitleBarHeight = 64.0f;

            const glm::vec2& m_IconSize {m_TitleBarHeight, m_TitleBarHeight};
            const glm::vec2& m_IconUV0 {0, 0};
            const glm::vec2& m_IconUV1 {0.5f, 1.0f};

            const glm::vec2& m_ControlBtnSize {20, 20};
            const glm::vec2& m_MinimizeUV0 {0.75f, 0.0f};
            const glm::vec2& m_MinimizeUV1 {1.0f, 0.5f};

            const glm::vec2& m_MaximizeUV0 {0.5f, 0.0f};
            const glm::vec2& m_MaximizeUV1 {0.75f, 0.5f};

            const glm::vec2& m_RestoreUV0 {0.5f, 0.5f};
            const glm::vec2& m_RestoreUV1 {0.75f, 1.0f};

            const glm::vec2& m_CloseUV0 {0.75f, 0.5f};
            const glm::vec2& m_CloseUV1 {1, 1};
        };
    }  // namespace Rendering
}  // namespace Scribe