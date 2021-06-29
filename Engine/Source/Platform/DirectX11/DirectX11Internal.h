#pragma once

#include "Sentinel/Base/Define.h"

#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>

// Forward declaration
struct GLFWwindow;

namespace Sentinel
{
	class DirectX11Internal {
	public:
		~DirectX11Internal();

		void Init(GLFWwindow* windowHandle);
		static DirectX11Internal* GetInternalHandle();

		Microsoft::WRL::ComPtr<ID3D11Device> GetDirectX11Device() const { return m_Device; }
		Microsoft::WRL::ComPtr<IDXGISwapChain> GetDirectX11SwapChain() const { return m_Swapchain; }
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDirectX11DeviceContext() const { return m_Context; }
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetDirectX11RenderTargetView() const { return m_Target; }

		Microsoft::WRL::ComPtr<IDXGIDevice> GetDirectX11DXGIDevice() const { return m_DXGIDevice; }
		Microsoft::WRL::ComPtr<IDXGIAdapter> GetDirectX11Adapter() const { return m_Adapter; }

		DXGI_ADAPTER_DESC* GetDirectX11AdapterDescription() const { return m_AdapterDescription; }

	private:
		DirectX11Internal() = default;
	private:
		static DirectX11Internal* s_Internal;
	private:
		GLFWwindow* m_WindowHandle;

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_Swapchain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_Target = nullptr;

		Microsoft::WRL::ComPtr<IDXGIDevice> m_DXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> m_Adapter = nullptr;

		DXGI_ADAPTER_DESC* m_AdapterDescription = nullptr;
	};
}
