#pragma once

#include "Sentinel/Base/Define.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

namespace Sentinel
{
	class DX11Common {
	public:
		inline static ID3D11Device* GetDevice() { return m_Device; }
		inline static ID3D11DeviceContext* GetContext() { return m_Context; }

		inline static IDXGIDevice* GetDXGIDevice() { return m_DXGIDevice; }
		inline static IDXGIAdapter* GetAdapter() { return m_Adapter; }

		inline static IDXGISwapChain* GetSwapchain() { return m_Swapchain; }

	private:
		static ID3D11Device* m_Device;
		static ID3D11DeviceContext* m_Context;

		static IDXGIDevice* m_DXGIDevice;
		static IDXGIAdapter* m_Adapter;

		static IDXGISwapChain* m_Swapchain;

		static ID3D11RenderTargetView* m_BackBuffer;
	};
}
