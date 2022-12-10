#pragma once

#include <Sentinel.h>

#include <Sentinel/Memory/PoolAllocator.h>

namespace Sentinel {
    struct Texture2DData;
    struct ContextData;
}  // namespace Sentinel

namespace Scribe {
    namespace Window {
        class ScribeWindow;
    }

    namespace Rendering {
        class ScribeImGuiBase final {
        public:
            ScribeImGuiBase(Sentinel::ContextData* context, Window::ScribeWindow* window);
            ~ScribeImGuiBase();

            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnRender();
            void OnImGuiRender();
            void OnPostRender();

        private:
            void RenderImGuiTitleBar();

        private:
            Sentinel::PoolAllocator<Sentinel::Texture2DData> m_TexMemAllocator;

            Sentinel::ContextData* m_Context;
            Window::ScribeWindow* m_Window;

            Sentinel::Texture2DData* m_LogoTex;
            Sentinel::Texture2DData* m_MinimizeTex;
            Sentinel::Texture2DData* m_MaximizeTex;
            Sentinel::Texture2DData* m_RestoreDownTex;
            Sentinel::Texture2DData* m_CloseTex;
        };
    }  // namespace Rendering
}  // namespace Scribe