#pragma once

#include <Sentinel.h>
#include <Sentinel/Memory/FixedSlabAllocator.h>

#include <Sentinel/Graphics/Output/ViewportData.h>

#include <Sentinel/Graphics/Output/FramebufferData.h>

#include <Sentinel/Graphics/Buffer/ConstantbufferData.h>
#include <Sentinel/Graphics/Buffer/VertexbufferData.h>
#include <Sentinel/Graphics/Buffer/IndexbufferData.h>
#include <Sentinel/Graphics/Buffer/VertexbufferLayoutData.h>

#include <Sentinel/Graphics/Material/ShaderData.h>

#include <Sentinel/Graphics/Texture/Texture2DData.h>
#include <Sentinel/Graphics/Texture/RenderTexture2DData.h>
#include <Sentinel/Graphics/Texture/DepthTexture2DData.h>

namespace Sentinel {
    class Window;
    class Camera;
    struct ContextData;
    struct SwapchainData;
}  // namespace Sentinel

namespace Scribe {
    namespace Rendering {
        class SceneRenderer final {
        public:
            SceneRenderer(Sentinel::Window* window, Sentinel::ContextData* context, Sentinel::SwapchainData* swapchain);
            ~SceneRenderer();

            Sentinel::RenderTexture2DData* GetFinalRT();
            Sentinel::UInt16 GetFinalRTWidth();
            Sentinel::UInt16 GetFinalRTHeight();

            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnRender();
            void OnPostRender();

            void Resize(Sentinel::UInt16 width, Sentinel::UInt16 height);

        private:
            Sentinel::Window* m_Window;

            Sentinel::FixedSlabAllocator<Sentinel::ViewportData> m_VPortAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::FramebufferData> m_FBufferAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::VertexbufferData> m_VBufferAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::IndexbufferData> m_IBufferAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::VertexbufferLayoutData> m_VLayoutAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::ConstantbufferData> m_CBufferAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::RenderTexture2DData> m_RTAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::DepthTexture2DData> m_DTAlloc;

            Sentinel::FixedSlabAllocator<Sentinel::ShaderData> m_ShaderAlloc;
            Sentinel::FixedSlabAllocator<Sentinel::Texture2DData> m_TexAlloc;

            Sentinel::Camera* m_Camera;

            Sentinel::ContextData* m_Context = nullptr;
            Sentinel::SwapchainData* m_Swapchain = nullptr;
            Sentinel::ViewportData* m_Viewport = nullptr;

            Sentinel::FramebufferData* m_FBuffer = nullptr;

            Sentinel::VertexbufferData* m_VBuffer = nullptr;
            Sentinel::IndexbufferData* m_IBuffer = nullptr;
            Sentinel::VertexbufferLayoutData* m_VLayout = nullptr;
            Sentinel::ConstantbufferData* m_CamCBuffer = nullptr;

            Sentinel::ShaderData* m_Shader = nullptr;
            Sentinel::Texture2DData* m_TestTexture = nullptr;

            Sentinel::UInt16 m_ResizeIndex = 0;
            glm::mat4 m_ViewProj = glm::mat4(1.0f);
        };
    }  // namespace Rendering
}  // namespace Scribe