#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Device/DX11SwapchainAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"
#include "Platform/DirectX11/Graphics/Texture/DX11RenderTexture2DAPI.h"
#include "Platform/DirectX11/Graphics/Texture/DX11DepthTexture2DAPI.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Sentinel {
    DX11SwapchainAPI::_init DX11SwapchainAPI::_initializer;

    void DX11SwapchainAPI::SwapBuffers(SwapchainData* dataObject) {
        DX11SwapchainData* swapchain = SwapchainAPI::Cast<DX11SwapchainData>(dataObject);
        swapchain->m_Swapchain->Present(swapchain->vSync ? 1 : 0, 0);
    }

    void DX11SwapchainAPI::Resize(SwapchainData* dataObject, UInt32 width, UInt32 height) {
        DX11SwapchainData* swapchain = SwapchainAPI::Cast<DX11SwapchainData>(dataObject);
        swapchain->m_Swapchain->ResizeBuffers(swapchain->m_BufferCount, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    }

    void DX11SwapchainAPI::Bind(SwapchainData* dataObject) {
        DX11SwapchainData* swapchain = SwapchainAPI::Cast<DX11SwapchainData>(dataObject);
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11RenderTexture2DData* renderTexture =
            RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(swapchain->backbuffer);

        DX11DepthTexture2DData* depthTexture = DepthTexture2DAPI::Cast<DX11DepthTexture2DData>(swapchain->depthBuffer);

        ID3D11DeviceContext* nativeContext = DX11ContextAPI::GetNativeContext(context);
        nativeContext->OMSetRenderTargets(1, &renderTexture->m_NativeRTV, depthTexture->m_NativeDSV);
    }

    void DX11SwapchainAPI::Unbind(SwapchainData* dataObject) {
        DX11SwapchainData* swapchain = SwapchainAPI::Cast<DX11SwapchainData>(dataObject);
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* nativeContext = DX11ContextAPI::GetNativeContext(context);
        ID3D11RenderTargetView* null = nullptr;
        nativeContext->OMSetRenderTargets(1, &null, NULL);
    }

    void DX11SwapchainAPI::Init(DX11SwapchainData* dataObject, GLFWwindow* windowHandle) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
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

        DX11ContextAPI::GetFactory(context)->CreateSwapChain(
            DX11ContextAPI::GetDevice(context), &swapChainDescription, &(dataObject->m_Swapchain));
    }

}  // namespace Sentinel