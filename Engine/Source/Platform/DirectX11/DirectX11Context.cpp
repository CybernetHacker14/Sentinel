#include "stpch.h"
#include "Platform/DirectX11/DirectX11Context.h"
#include "Platform/DirectX11/DirectX11Internal.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Sentinel
{
	DirectX11Context::DirectX11Context(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle) {
		ST_ENGINE_ASSERT(windowHandle, "Window Handle is null!");
	}

	void DirectX11Context::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		DirectX11Internal::GetInternalHandle()->Init(m_WindowHandle);

		// Getting context information for DirectX is complex and different as compared to OpenGL

		char videoCardDescription[128];

		STL::string vendor, major, minor, release, build, version;
		LARGE_INTEGER driverVersion;

		wcstombs_s(NULL, videoCardDescription, 128,
			DirectX11Internal::GetInternalHandle()->GetDirectX11AdapterDescription()->Description, 128);

		if (DirectX11Internal::GetInternalHandle()->GetDirectX11AdapterDescription()->VendorId == 0x10DE)
			vendor = "NVIDIA Corporation";
		else if (DirectX11Internal::GetInternalHandle()->GetDirectX11AdapterDescription()->VendorId == 0x1002)
			vendor = "AMD";
		else if (DirectX11Internal::GetInternalHandle()->GetDirectX11AdapterDescription()->VendorId == 0x8086)
			vendor = "Intel";
		else if (DirectX11Internal::GetInternalHandle()->GetDirectX11AdapterDescription()->VendorId == 0x1414)
			vendor = "Microsoft";
		else
			vendor = "Unknown vendor!";

		DirectX11Internal::GetInternalHandle()->GetDirectX11Adapter()->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);

		m_ContextInfo.Vendor = (unsigned char*)vendor.c_str();
		m_ContextInfo.Renderer = (unsigned char*)videoCardDescription;
		m_ContextInfo.API = (unsigned char*)"DirectX 11";

		major = std::to_string(HIWORD(driverVersion.HighPart)).c_str();
		minor = std::to_string(LOWORD(driverVersion.HighPart)).c_str();
		release = std::to_string(HIWORD(driverVersion.LowPart)).c_str();
		build = std::to_string(LOWORD(driverVersion.LowPart)).c_str();

		version = major + "." + minor + "." + release + "." + build;

		m_ContextInfo.Version = (unsigned char*)version.c_str();

		ST_ENGINE_INFO("VENDOR   : {0}", m_ContextInfo.Vendor);
		ST_ENGINE_INFO("RENDERER : {0}", m_ContextInfo.Renderer);
		ST_ENGINE_INFO("API      : {0}", m_ContextInfo.API);
		ST_ENGINE_INFO("VERSION  : {0}", m_ContextInfo.Version);
	}

	void DirectX11Context::SwapBuffers() {
		DirectX11Internal::GetInternalHandle()->GetDirectX11SwapChain()->Present(0, 0);
	}
}
