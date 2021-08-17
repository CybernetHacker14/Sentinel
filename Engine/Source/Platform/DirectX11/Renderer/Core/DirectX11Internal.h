#pragma once

#include "Sentinel/Base/Define.h"

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

// Forward declaration
struct GLFWwindow;

namespace Sentinel
{
	struct Vertex {
		float x, y, z;
	};

	class DirectX11Internal {
	public:
		~DirectX11Internal();

		void Init(GLFWwindow* windowHandle);

		void Draw();

		static DirectX11Internal* GetInternalHandle() { return s_Internal; }

		void CreateDeviceAndSwapchain(GLFWwindow* windowHandle);
		void SetViewport(UInt x, UInt y, UInt width, UInt height);

		void CreateRenderTargetView();

		void ClearColor(const float* color);

		void ReleaseAndCleanObjects();
	public:
		ID3D11Device* GetDevice() const { return m_Device; }
		IDXGISwapChain* GetSwapChain() const { return m_Swapchain; }
		ID3D11DeviceContext* GetDeviceContext() const { return m_Context; }
		ID3D11RenderTargetView* GetBackBuffer() const { return m_BackBuffer; }

		IDXGIDevice* GetDXGIDevice() const { return m_DXGIDevice; }
		IDXGIAdapter* GetAdapter() const { return m_Adapter; }

		DXGI_ADAPTER_DESC* GetAdapterDescription() const { return m_AdapterDescription; }

	private:
		DirectX11Internal() = default;
	private:
		static DirectX11Internal* s_Internal;
	private:
		ID3D11Device* m_Device = nullptr;
		IDXGISwapChain* m_Swapchain = nullptr;
		ID3D11DeviceContext* m_Context = nullptr;
		ID3D11RenderTargetView* m_BackBuffer = nullptr;

		IDXGIDevice* m_DXGIDevice = nullptr;
		IDXGIAdapter* m_Adapter = nullptr;

		DXGI_ADAPTER_DESC* m_AdapterDescription = nullptr;

		ID3D11VertexShader* pVS;
		ID3D11PixelShader* pPS;

		ID3D11Buffer* pVBuffer;
	};
}
