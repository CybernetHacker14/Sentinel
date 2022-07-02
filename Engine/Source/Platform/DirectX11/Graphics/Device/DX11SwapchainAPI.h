#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Device/SwapchainAPI.h"

struct GLFWwindow;

namespace Sentinel {
    struct DX11SwapchainData;

    class DX11SwapchainAPI final: public SwapchainAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) { m_SwapFunction = DX11SwapchainAPI::SwapBuffers; }
            }
        } _initializer;

        static void SwapBuffers(SwapchainData* dataObject);

    private:
        static void Init(DX11SwapchainData* dataObject, GLFWwindow* windowHandle);

    private:
        friend class SwapchainAPI;
    };
}  // namespace Sentinel