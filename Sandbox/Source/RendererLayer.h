#pragma once

#include <Sentinel.h>
#include <Sentinel/Memory/PoolAllocator.h>

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

    class ImGuiLayer;
    class ImGuiDebugLayer;
}  // namespace Sentinel

namespace Sandbox {
    namespace Rendering {
        class RendererLayer final {
        public:
            RendererLayer(Sentinel::Window* window);
            ~RendererLayer();

            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnRender();
            void OnImGuiRender();
            void OnPostRender();

        private:
            void OnWindowResize(Sentinel::Event& event);
            void Resize(Sentinel::UInt16 width, Sentinel::UInt16 height);

        public:
            Sentinel::ContextData* m_Context;

        private:
            Sentinel::PoolAllocator<Sentinel::ContextData> m_CtxAlloc;
            Sentinel::PoolAllocator<Sentinel::SwapchainData> m_SCAlloc;
            Sentinel::PoolAllocator<Sentinel::ViewportData> m_VPortAlloc;

            Sentinel::PoolAllocator<Sentinel::VertexbufferData> m_VBufferAlloc;
            Sentinel::PoolAllocator<Sentinel::IndexbufferData> m_IBufferAlloc;
            Sentinel::PoolAllocator<Sentinel::VertexbufferLayoutData> m_LayoutAlloc;
            Sentinel::PoolAllocator<Sentinel::ConstantbufferData> m_CBufferAlloc;

            Sentinel::PoolAllocator<Sentinel::ShaderData> m_ShaderAlloc;
            Sentinel::PoolAllocator<Sentinel::Texture2DData> m_TexAlloc;

            Sentinel::PoolAllocator<Sentinel::RenderTexture2DData> m_RTAlloc;
            Sentinel::PoolAllocator<Sentinel::DepthTexture2DData> m_DTAlloc;

            Sentinel::SharedRef<Sentinel::Camera> m_Camera;

            Sentinel::Window* m_Window;
            Sentinel::SwapchainData* m_Swapchain;
            Sentinel::ViewportData* m_Viewport;

            Sentinel::VertexbufferData* m_VBuffer;
            Sentinel::IndexbufferData* m_IBuffer;
            Sentinel::VertexbufferLayoutData* m_VLayout;

            Sentinel::ShaderData* m_Shader;
            Sentinel::Texture2DData* m_Texture;

            Sentinel::RenderTexture2DData* m_RenderTexture;
            Sentinel::DepthTexture2DData* m_DepthTexture;

        private:
            Sentinel::UInt32 m_ResizeIndex = 0;
        };
    }  // namespace Rendering
}  // namespace Sandbox