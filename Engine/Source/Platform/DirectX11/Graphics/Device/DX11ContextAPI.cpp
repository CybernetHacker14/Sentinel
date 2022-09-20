#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

#include <GLFW/glfw3.h>

namespace Sentinel {
    DX11ContextAPI::_init DX11ContextAPI::_initializer;

    void DX11ContextAPI::Draw(ContextData* dataObject) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject);
        context->m_Context->Draw(0, 0);
    }

    void DX11ContextAPI::DrawIndexed(ContextData* dataObject, const UInt32 count) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject);
        context->m_Context->DrawIndexed(count, 0, 0);
    }

    void DX11ContextAPI::Clean(ContextData* dataObject) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject);
        context->m_Context->Release();
        context->m_Device->Release();
        context->m_DXGIDevice->Release();
        context->m_Factory->Release();
        context->m_Adapter->Release();
    }

    void DX11ContextAPI::Create(DX11ContextData* dataObject, GLFWwindow* windowHandle) {
        if (dataObject->m_ContextType == ContextType::IMMEDIATE) {
            D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                D3D11_CREATE_DEVICE_DEBUG,
                nullptr,
                0,
                D3D11_SDK_VERSION,
                &(dataObject->m_Device),
                nullptr,
                &(dataObject->m_Context));

            dataObject->m_Device->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&(dataObject->m_DXGIDevice));
            dataObject->m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (LPVOID*)&(dataObject->m_Adapter));
            dataObject->m_Adapter->GetParent(__uuidof(IDXGIFactory), (LPVOID*)&(dataObject->m_Factory));

            char videoCardDescription[128];
            STL::string vendor, major, minor, release, build, version;
            LARGE_INTEGER driverVersion;

            DXGI_ADAPTER_DESC adapterDescription;
            SecureZeroMemory(&adapterDescription, sizeof(DXGI_ADAPTER_DESC));

            dataObject->m_Adapter->GetDesc(&adapterDescription);
            wcstombs_s(NULL, videoCardDescription, 128, adapterDescription.Description, 128);

            switch (adapterDescription.VendorId) {
                case 0x10DE: vendor = "NVIDIA Corporation"; break;
                case 0x1002: vendor = "AMD Inc."; break;
                case 0x8086: vendor = "Intel"; break;
                case 0x1414: vendor = "Microsoft"; break;
                default: vendor = "Unknown vendor"; break;
            }

            dataObject->m_Adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);

            dataObject->m_ContextInfo.Vendor = vendor;
            dataObject->m_ContextInfo.Renderer = videoCardDescription;
            dataObject->m_ContextInfo.API = "DirectX 11";

            major = STL::to_string(HIWORD(driverVersion.HighPart));
            minor = STL::to_string(LOWORD(driverVersion.HighPart));
            release = STL::to_string(HIWORD(driverVersion.LowPart));
            build = STL::to_string(LOWORD(driverVersion.LowPart));

            version = major + "." + minor + "." + release + "." + build;

            dataObject->m_ContextInfo.Version = version;

            glfwMakeContextCurrent(windowHandle);

        } else if (dataObject->m_ContextType == ContextType::DEFFERED) {
            // TODO : R&D and implement this
        }

        D3D11_BLEND_DESC blendDesc;
        SecureZeroMemory(&blendDesc, sizeof(blendDesc));
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.RenderTarget[0].BlendEnable = true;

        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        dataObject->m_Device->CreateBlendState(&blendDesc, &(dataObject->m_BlendState));
        dataObject->m_Context->OMSetBlendState(dataObject->m_BlendState, nullptr, 0xffffffff);
    }
}  // namespace Sentinel