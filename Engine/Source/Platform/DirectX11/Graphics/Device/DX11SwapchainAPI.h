#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Device/SwapchainAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11SwapchainData.h"

namespace Sentinel {
    struct DX11SwapchainData;

    class DX11SwapchainAPI final: public SwapchainAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_SwapFunction = DX11SwapchainAPI::SwapBuffers;
                    m_ResizeFunction = DX11SwapchainAPI::Resize;
                    m_BindFunction = DX11SwapchainAPI::Bind;
                    m_UnbindFunction = DX11SwapchainAPI::Unbind;
                }
            }
        } _initializer;

    public:
        inline static IDXGISwapChain* GetNativeSwapchain(DX11SwapchainData* dataObject) {
            return dataObject->m_Swapchain;
        }

    private:
        static void SwapBuffers(SwapchainData* dataObject);
        static void Resize(SwapchainData* dataObject, UInt32 width, UInt32 height);

        static void Bind(SwapchainData* dataObject);
        static void Unbind(SwapchainData* dataObject);

    private:
        static void Init(DX11SwapchainData* dataObject, GLFWwindow* windowHandle);

    private:
        friend class SwapchainAPI;
    };
}  // namespace Sentinel