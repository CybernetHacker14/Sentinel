#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/SwapchainData.h"

struct GLFWwindow;

namespace Sentinel {
    class GraphicsMemoryManager;

    class SwapchainAPI {
    public:
        static SwapchainData* CreateSwapchain(
            SharedRef<GraphicsMemoryManager> memoryHandle, ContextData* context, GLFWwindow* windowHandle);

        inline static void SwapBuffers(SwapchainData* dataObject) {
            if (!m_SwapFunction) return;
            m_SwapFunction(dataObject);
        }

        inline static void Resize(SwapchainData* dataObject, UInt32 width, UInt32 height) {
            if (!m_ResizeFunction) return;
            m_ResizeFunction(dataObject, width, height);
        }

        inline static void Bind(SwapchainData* dataObject) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject);
        }

        inline static void Unbind(SwapchainData* dataObject) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject);
        }

        // Relaying the creation and deletion to external components, cause creating render textures and depth textures
        // requires a lot of parameters in place, which breaks the API modularity.
        // So as of now, Swapchain backbuffer creation is EXPLICIT.
        inline static void SetBuffers(
            SwapchainData* dataObject, RenderTexture2DData* renderTexture, DepthTexture2DData* depthTexture) {
            if (!m_SetFunction) return;
            m_SetFunction(dataObject, renderTexture, depthTexture);
        }

        // Since the creation is external, maybe the deletion can be external as well, and we just null the buffer
        // pointers here
        inline static void UnsetBuffers(SwapchainData* dataObject) {
            if (!m_UnsetFunction) return;
            m_UnsetFunction(dataObject);
        }

    public:
        template<typename T>
        inline static T* Cast(SwapchainData* dataObject) {
            static_assert(STL::is_base_of<SwapchainData, T>::value, "'T' should be derived from SwapchainData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(SwapchainData*, RenderTexture2DData*, DepthTexture2DData*)> m_SetFunction;
        inline static STL::delegate<void(SwapchainData*)> m_UnsetFunction;
        inline static STL::delegate<void(SwapchainData*)> m_SwapFunction;
        inline static STL::delegate<void(SwapchainData*, UInt32, UInt32)> m_ResizeFunction;
        inline static STL::delegate<void(SwapchainData*)> m_BindFunction;
        inline static STL::delegate<void(SwapchainData*)> m_UnbindFunction;
    };
}  // namespace Sentinel