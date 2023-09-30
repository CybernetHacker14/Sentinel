#pragma once

#include <Sentinel.h>
#include <Sentinel/Memory/FixedSlabAllocator.h>

#include <Sentinel/Graphics/Device/ContextAPI.h>
#include <Sentinel/Graphics/Device/SwapchainAPI.h>
#include <Sentinel/Graphics/Output/ViewportAPI.h>

#include <Sentinel/Graphics/Texture/RenderTexture2DAPI.h>
#include <Sentinel/Graphics/Texture/DepthTexture2DAPI.h>

namespace Sentinel {
    class Wwindow;
}

namespace Scribe {
    namespace Rendering {
        class SceneRenderer final {
        public:
            SceneRenderer(Sentinel::Window* window);

            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnRender();
            void OnPostRender();

        private:
            Sentinel::Window* m_Window;

            Sentinel::FixedSlabAllocator<Sentinel::ContextData> m_CtxAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::SwapchainData> m_SCAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::ViewportData> m_VPortAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::RenderTexture2DData> m_RTAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::DepthTexture2DData> m_DTAlloc;

            Sentinel::ContextData* m_Context = nullptr;
            Sentinel::SwapchainData* m_Swapchain = nullptr;
            Sentinel::ViewportData* m_Viewport = nullptr;

            Sentinel::RenderTexture2DData* m_RenderTexture = nullptr;
            Sentinel::DepthTexture2DData* m_DepthTexture = nullptr;
        };
    }  // namespace Rendering
}  // namespace Scribe