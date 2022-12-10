#include "stpch.h"

#ifdef ST_RENDERER_DX11

    #include "Sentinel/Graphics/Device/SwapchainAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"
    #include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"
    #include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3.h>
    #include <GLFW/glfw3native.h>

namespace Sentinel {
    SwapchainData* Sentinel::SwapchainAPI::CreateSwapchain(
        PoolAllocator<SwapchainData>& allocator, ContextData* context, GLFWwindow* windowHandle) {
        SwapchainData* swapchain = allocator.New();
        swapchain->Context = context;
        Init(swapchain, windowHandle);
        return swapchain;
    }

    void SwapchainAPI::SwapBuffers(SwapchainData* dataObject) {
        dataObject->m_Swapchain->Present(dataObject->vSync ? 1 : 0, 0);
    }

    void SwapchainAPI::Resize(SwapchainData* dataObject, UInt16 width, UInt16 height) {
        dataObject->m_Swapchain->ResizeBuffers(dataObject->m_BufferCount, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    }

    void SwapchainAPI::Bind(SwapchainData* dataObject) {
        ID3D11DeviceContext* nativeContext = ContextAPI::GetNativeContext(dataObject->Context);

        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRV[1];
        pRV[0] = RenderTexture2DAPI::GetNativeRTV(dataObject->backbuffer);
        ID3D11DepthStencilView* pDV = NULL;
        if (dataObject->depthBuffer) { pDV = DepthTexture2DAPI::GetNativeDSV(dataObject->depthBuffer); }

        if (pRV) { nativeContext->OMSetRenderTargets(1, pRV[0].GetAddressOf(), pDV); }
    }

    void SwapchainAPI::Unbind(SwapchainData* dataObject) {
        ID3D11DeviceContext* nativeContext = ContextAPI::GetNativeContext(dataObject->Context);
        ID3D11RenderTargetView* null = nullptr;
        nativeContext->OMSetRenderTargets(1, &null, NULL);
    }

    void SwapchainAPI::SetBuffers(
        SwapchainData* dataObject, RenderTexture2DData* renderTexture, DepthTexture2DData* depthTexture) {
        dataObject->backbuffer = renderTexture;
        RenderTexture2DAPI::SetSwapchainTarget(renderTexture, true);
        if (depthTexture) {
            dataObject->depthBuffer = depthTexture;
            DepthTexture2DAPI::SetSwapchainTarget(depthTexture, true);
        }
    }

    void SwapchainAPI::UnsetBuffers(SwapchainData* dataObject) {
        RenderTexture2DAPI::SetSwapchainTarget(dataObject->backbuffer, false);
        DepthTexture2DAPI::SetSwapchainTarget(dataObject->depthBuffer, false);
        dataObject->backbuffer = nullptr;
        dataObject->depthBuffer = nullptr;
    }

    void SwapchainAPI::Clean(SwapchainData* dataObject) {
        dataObject->m_Swapchain->SetFullscreenState(false, NULL);
        dataObject->m_Swapchain->Release();
        dataObject->m_Swapchain = 0;
    }

    void SwapchainAPI::Init(SwapchainData* dataObject, GLFWwindow* windowHandle) {
        DXGI_SWAP_CHAIN_DESC swapChainDescription;
        SecureZeroMemory(&swapChainDescription, sizeof(swapChainDescription));

        swapChainDescription.BufferDesc.Width = 0;
        swapChainDescription.BufferDesc.Height = 0;
        swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDescription.BufferDesc.RefreshRate.Denominator = 0;
        swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        swapChainDescription.SampleDesc.Count = 4;
        swapChainDescription.SampleDesc.Quality = 0;

        swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        swapChainDescription.BufferCount = dataObject->m_BufferCount = 1;

        swapChainDescription.OutputWindow = glfwGetWin32Window(windowHandle);

        swapChainDescription.Windowed = true;

        swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDescription.Flags = 0;

        ContextAPI::GetFactory(dataObject->Context)
            ->CreateSwapChain(
                ContextAPI::GetDevice(dataObject->Context), &swapChainDescription, &(dataObject->m_Swapchain));
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11