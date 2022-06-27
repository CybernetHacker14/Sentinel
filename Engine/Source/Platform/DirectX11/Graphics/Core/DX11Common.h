#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11shadertracing.h>
#include <wrl.h>

namespace Sentinel {
    class DX11RenderPipeline;
    class DX11GraphicsContext;

    class DX11Common {
    public:
        static ID3D11Device* GetDevice();
        static ID3D11DeviceContext* GetContext();

        static IDXGIDevice* GetDXGIDevice();
        static IDXGIAdapter* GetAdapter();

        static IDXGISwapChain* GetSwapchain();

    private:
        friend class DX11RenderPipeline;
        friend class DX11GraphicsContext;

        static ID3D11Device* m_Device;
        static ID3D11DeviceContext* m_Context;

        static IDXGIDevice* m_DXGIDevice;
        static IDXGIAdapter* m_Adapter;

        static IDXGISwapChain* m_Swapchain;
    };
}  // namespace Sentinel
