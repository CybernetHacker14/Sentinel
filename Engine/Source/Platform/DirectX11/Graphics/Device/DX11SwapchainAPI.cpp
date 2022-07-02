#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Device/DX11SwapchainAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11SwapchainData.h"

namespace Sentinel {
    DX11SwapchainAPI::_init DX11SwapchainAPI::_initializer;

    void DX11SwapchainAPI::SwapBuffers(SwapchainData* dataObject) {
        DX11SwapchainData* swapchain = SwapchainAPI::Cast<DX11SwapchainData>(dataObject);
        swapchain->m_Swapchain->Present(SwapchainAPI::GetVSync(dataObject) ? 1 : 0, 0);
    }

    void DX11SwapchainAPI::Init(DX11SwapchainData* dataObject, GLFWwindow* windowHandle) {

    }

}  // namespace Sentinel