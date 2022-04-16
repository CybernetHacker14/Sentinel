#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Components/FrameBackings/Misc/DX11GraphicsContext.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Sentinel
{
	DX11GraphicsContext::DX11GraphicsContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle) {
		ST_ENGINE_ASSERT(m_WindowHandle, "Window Handle is null");

		m_InitFunction = ST_BIND_EVENT_FN(Init);

		InitializeDeviceAndSwapchain();
		InitializeDirectXComponents();
		glfwMakeContextCurrent(m_WindowHandle);
	}

	void DX11GraphicsContext::Init() {
		SetContextInfo();
	}

	void DX11GraphicsContext::InitializeDeviceAndSwapchain() {
		DXGI_SWAP_CHAIN_DESC swapChainDescription;
		SecureZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

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

		swapChainDescription.BufferCount = 1;

		swapChainDescription.OutputWindow = glfwGetWin32Window(m_WindowHandle);

		swapChainDescription.Windowed = true;

		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDescription.Flags = 0;

		auto result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapChainDescription,
			&(DX11Common::m_Swapchain),
			&(DX11Common::m_Device),
			nullptr,
			&(DX11Common::m_Context)
		);
	}

	void DX11GraphicsContext::InitializeDirectXComponents() {
		DX11Common::GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&(DX11Common::m_DXGIDevice));
		DX11Common::GetDXGIDevice()->GetParent(__uuidof(IDXGIAdapter), (LPVOID*)&(DX11Common::m_Adapter));
	}

	void DX11GraphicsContext::SetContextInfo() {
		char videoCardDescription[128];
		STL::string vendor, major, minor, release, build, version;
		LARGE_INTEGER driverVersion;

		DXGI_ADAPTER_DESC adapterDescription;
		SecureZeroMemory(&adapterDescription, sizeof(DXGI_ADAPTER_DESC));

		DX11Common::GetAdapter()->GetDesc(&adapterDescription);

		wcstombs_s(NULL, videoCardDescription, 128, adapterDescription.Description, 128);

		switch (adapterDescription.VendorId)
		{
			case 0x10DE:
				vendor = "NVIDIA Corporation";
				break;
			case 0x1002:
				vendor = "AMD Inc.";
				break;
			case 0x8086:
				vendor = "Intel";
				break;
			case 0x1414:
				vendor = "Microsoft";
				break;
			default:
				vendor = "Unknown vendor";
				break;
		}

		DX11Common::GetAdapter()->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);

		m_ContextInfo.Vendor = vendor;
		m_ContextInfo.Renderer = videoCardDescription;
		m_ContextInfo.API = "DirectX 11";

		major = STL::to_string(HIWORD(driverVersion.HighPart));
		minor = STL::to_string(LOWORD(driverVersion.HighPart));
		release = STL::to_string(HIWORD(driverVersion.LowPart));
		build = STL::to_string(LOWORD(driverVersion.LowPart));

		version = major + "." + minor + "." + release + "." + build;

		m_ContextInfo.Version = version;
	}
}
