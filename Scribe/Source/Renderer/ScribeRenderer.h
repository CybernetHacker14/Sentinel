#pragma once

#include <Sentinel.h>
#include <Sentinel/Memory/FixedSlabAllocator.h>

#include <Sentinel/Graphics/Device/ContextAPI.h>
#include <Sentinel/Graphics/Device/SwapchainAPI.h>

#include <Sentinel/Graphics/Texture/RenderTexture2DAPI.h>

namespace Sentinel {
    class Window;
}  // namespace Sentinel

namespace Scribe {
    namespace Rendering {
        class ScribeRenderer final {
        public:
            ScribeRenderer(Sentinel::Window* window);
            ~ScribeRenderer();

            inline Sentinel::ContextData* GetRenderingContext() { return m_Context; }

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