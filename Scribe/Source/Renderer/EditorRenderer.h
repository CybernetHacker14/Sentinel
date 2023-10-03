#pragma once

#include <Sentinel.h>
#include <Sentinel/Memory/FixedSlabAllocator.h>

#include <Sentinel/Graphics/Device/ContextData.h>
#include <Sentinel/Graphics/Device/SwapchainData.h>

#include <Sentinel/Graphics/Texture/RenderTexture2DData.h>

namespace Sentinel {
    class Window;
}  // namespace Sentinel

namespace Scribe {
    namespace Rendering {

        // This is the base minimum class for rendering the Editor and using ImGui
        // This holds the context, the swpachain and the RT data
        // Context is supplied to ImGui from here
        class EditorRenderer final {
        public:
            EditorRenderer(Sentinel::Window* window);
            ~EditorRenderer();

            inline Sentinel::ContextData* GetContext() { return m_Context; }
            inline Sentinel::SwapchainData* GetSwapchain() { return m_Swapchain; }

            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnRender();
            void OnPostRender();

            Sentinel::Bool OnWindowResize(Sentinel::EventType type, Sentinel::EventData data, void* listener);
            void Resize(Sentinel::UInt16 width, Sentinel::UInt16 height);

        private:
            Sentinel::FixedSlabAllocator<Sentinel::ContextData> m_CtxAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::SwapchainData> m_SCAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::RenderTexture2DData> m_RTAlloc;

            Sentinel::Window* m_Window;

            Sentinel::ContextData* m_Context;
            Sentinel::SwapchainData* m_Swapchain;

            Sentinel::RenderTexture2DData* m_SwapchainRT;

        private:
            Sentinel::UInt16 m_ResizeIndex = 0;

            glm::mat4 m_ViewProj = glm::mat4(1.0f);
        };
    }  // namespace Rendering
}  // namespace Scribe