#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/SwapchainData.h"

namespace Sentinel {
    struct GLFWwindow;
    class GraphicsMemoryManager;

    class SwapchainAPI {
    public:
        static SwapchainData* CreateSwapchain(SharedRef<GraphicsMemoryManager> memoryHandle, GLFWwindow* windowHandle);

        inline static void SwapBuffers(SwapchainData* dataObject) {
            if (!m_SwapFunction) return;
            m_SwapFunction(dataObject);
        }

        inline static void SetVSync(SwapchainData* dataObject, Bool value) { dataObject->vSync = value; }
        inline static const Bool GetVSync(SwapchainData* dataObject) { return dataObject->vSync; }

    public:
        template<typename T>
        inline static T* Cast(SwapchainData* dataObject) {
            static_assert(STL::is_base_of<SwapchainData, T>::value, "'T' should be derived from SwapchainData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(SwapchainData*)> m_SwapFunction;
    };
}  // namespace Sentinel