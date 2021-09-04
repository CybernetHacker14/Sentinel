#include "stpch.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Modules/DX11GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Sentinel
{
	DX11GraphicsContext::DX11GraphicsContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle) {
		ST_ENGINE_ASSERT(m_WindowHandle, "Window Handle is null");
	}

	void DX11GraphicsContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		SetContextInfo();
	}

	void DX11GraphicsContext::SetContextInfo() {
		char videoCardDescription[128];
		STL::string vendor, major, minor, release, build, version;
		LARGE_INTEGER driverVersion;

		DXGI_ADAPTER_DESC adapterDescription;
		ZeroMemory(&adapterDescription, sizeof(DXGI_ADAPTER_DESC));

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
