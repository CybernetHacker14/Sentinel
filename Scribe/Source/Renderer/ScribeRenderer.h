#pragma once

#include <Sentinel.h>
#include <Sentinel/Memory/FixedSlabAllocator.h>

#include <Sentinel/Graphics/Device/ContextAPI.h>
#include <Sentinel/Graphics/Device/SwapchainAPI.h>
#include <Sentinel/Graphics/Output/ViewportAPI.h>

#include <Sentinel/Graphics/Buffer/VertexbufferAPI.h>
#include <Sentinel/Graphics/Buffer/IndexbufferAPI.h>
#include <Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h>
#include <Sentinel/Graphics/Buffer/ConstantbufferAPI.h>

#include <Sentinel/Graphics/Material/ShaderAPI.h>
#include <Sentinel/Graphics/Texture/Texture2DAPI.h>

#include <Sentinel/Graphics/Texture/RenderTexture2DAPI.h>
#include <Sentinel/Graphics/Texture/DepthTexture2DAPI.h>

namespace Sentinel {
    class Camera;
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
            Sentinel::FixedSlabAllocator<Sentinel::ViewportData> m_VPortAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::RenderTexture2DData> m_RTAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::DepthTexture2DData> m_DTAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::ShaderData> m_ShaderAlloc;

            Sentinel::Window* m_Window;

            Sentinel::ContextData* m_Context;
            Sentinel::SwapchainData* m_Swapchain;
            Sentinel::ViewportData* m_Viewport;

            Sentinel::ShaderData* m_Shader;

            Sentinel::RenderTexture2DData* m_SwapchainRT;
            Sentinel::DepthTexture2DData* m_SwapchainDRT;

        private:
            Sentinel::UInt16 m_ResizeIndex = 0;

            glm::mat4 m_ViewProj = glm::mat4(1.0f);
        };
    }  // namespace Rendering
}  // namespace Scribe