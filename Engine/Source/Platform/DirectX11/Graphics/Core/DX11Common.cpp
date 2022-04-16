#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	ID3D11Device* DX11Common::m_Device = nullptr;
	ID3D11DeviceContext* DX11Common::m_Context = nullptr;
	IDXGIDevice* DX11Common::m_DXGIDevice = nullptr;
	IDXGIAdapter* DX11Common::m_Adapter = nullptr;
	IDXGISwapChain* DX11Common::m_Swapchain = nullptr;

	ID3D11Device* DX11Common::GetDevice() {
		return m_Device;
	}

	ID3D11DeviceContext* DX11Common::GetContext() {
		return m_Context;
	}

	IDXGIDevice* DX11Common::GetDXGIDevice() {
		return m_DXGIDevice;
	}

	IDXGIAdapter* DX11Common::GetAdapter() {
		return m_Adapter;
	}

	IDXGISwapChain* DX11Common::GetSwapchain() {
		return m_Swapchain;
	}
}
