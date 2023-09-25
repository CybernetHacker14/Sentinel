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

#include <Sentinel/Resources/ImageResourceLoader.h>

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
            Sentinel::Bool OnWindowResize(Sentinel::EventType type, Sentinel::EventData data, void* listener);
            void Resize(Sentinel::UInt16 width, Sentinel::UInt16 height);

        public:
            Sentinel::ContextData* m_Context;

        private:
            Sentinel::FixedSlabAllocator<Sentinel::ContextData> m_CtxAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::SwapchainData> m_SCAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::ViewportData> m_VPortAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::VertexbufferData> m_VBufferAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::IndexbufferData> m_IBufferAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::VertexbufferLayoutData> m_LayoutAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::ConstantbufferData> m_CBufferAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::ShaderData> m_ShaderAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::Texture2DData> m_TexAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::RenderTexture2DData> m_RTAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::DepthTexture2DData> m_DTAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::ImageResourceLoader> m_ImageLoader;

            Sentinel::Camera* m_Camera;

            Sentinel::Window* m_Window;
            Sentinel::SwapchainData* m_Swapchain;
            Sentinel::ViewportData* m_Viewport;
            Sentinel::ConstantbufferData* m_CamCBuffer;

            Sentinel::VertexbufferData* m_VBuffer;
            Sentinel::IndexbufferData* m_IBuffer;
            Sentinel::VertexbufferLayoutData* m_VLayout;

            Sentinel::ShaderData* m_Shader;
            Sentinel::Texture2DData* m_Texture;

            Sentinel::RenderTexture2DData* m_RenderTexture;
            Sentinel::DepthTexture2DData* m_DepthTexture;

        private:
            Sentinel::UInt32 m_ResizeIndex = 0;

            glm::mat4 m_ViewProj = glm::mat4(1.0f);
        };
    }  // namespace Rendering
}  // namespace Sandbox