#include "stpch.h"
#include "Platform/DirectX11/DirectX11Internal.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Sentinel
{
	DirectX11Internal* DirectX11Internal::s_Internal = new DirectX11Internal();

	DirectX11Internal::~DirectX11Internal() {
		delete m_AdapterDescription;
		delete s_Internal;
	}

	void DirectX11Internal::Init(GLFWwindow* windowHandle) {
		DXGI_SWAP_CHAIN_DESC description;
		ZeroMemory(&description, sizeof(DXGI_SWAP_CHAIN_DESC));

		description.BufferDesc.Width = 0;
		description.BufferDesc.Height = 0;
		description.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		description.BufferDesc.RefreshRate.Numerator = 0;
		description.BufferDesc.RefreshRate.Denominator = 0;
		description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		description.SampleDesc.Count = 1;
		description.SampleDesc.Quality = 0;

		description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		description.BufferCount = 1;

		description.OutputWindow = glfwGetWin32Window(windowHandle);

		description.Windowed = true;

		description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		description.Flags = 0;

		// Create the device and the swapchain
		auto result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&description,
			&m_Swapchain,
			&m_Device,
			nullptr,
			&m_Context
		);

		m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_DXGIDevice);
		m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_Adapter);

		m_AdapterDescription = new DXGI_ADAPTER_DESC();
		m_Adapter->GetDesc(m_AdapterDescription);

		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer = nullptr;

		// In the GetBuffer function, 0 will give us the backbuffer
		m_Swapchain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
		m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_Target);
	}

	DirectX11Internal* DirectX11Internal::GetInternalHandle() {
		return s_Internal;
	}
}
