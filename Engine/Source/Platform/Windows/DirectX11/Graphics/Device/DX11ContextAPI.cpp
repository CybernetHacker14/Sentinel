#include "stpch.h"

#ifdef ST_RENDERER_DX11
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/Windows/DirectX11/Graphics/Core/DX11Common.h"
    #include "Sentinel/Common/Strings/String.h"

    #include <GLFW/glfw3.h>
    #include <sparse_map.h>

namespace Sentinel {

    namespace DX11ContextAPIUtils {
        static tsl::sparse_map<UInt32, Char*> s_VendorCodeMap = {
            {0x10DE, "NVIDIA Corporation"}, {0x1002, "AMD Inc."}, {0x8086, "Intel"}, {0x1414, "Microsoft"}};

        static CChar* ToString(Char* buffer, Int32 value) {
            sprintf_s(buffer, 12, "%d", value);
            return buffer;
        }
    }  // namespace DX11ContextAPIUtils

    ContextData* Sentinel::ContextAPI::CreateImmediateContext(
        FixedSlabAllocator<ContextData>& allocator, GLFWwindow* windowHandle) {
        ContextData* context = allocator.New();
        context->m_ContextType = ContextType::IMMEDIATE;
        Create(context, windowHandle);
        return context;
    }

    ContextData* ContextAPI::CreateDeferredContext(
        FixedSlabAllocator<ContextData>& allocator, GLFWwindow* windowHandle) {
        return nullptr;
    }

    void ContextAPI::Draw(ContextData* dataObject) {
        dataObject->m_Context->Draw(0, 0);
    }

    void ContextAPI::DrawIndexed(ContextData* dataObject, const UInt32 count) {
        dataObject->m_Context->DrawIndexed(count, 0, 0);
    }

    void ContextAPI::Clean(ContextData* dataObject) {
        dataObject->m_Context->Release();
        dataObject->m_Device->Release();
        dataObject->m_DXGIDevice->Release();
        dataObject->m_Factory->Release();
        dataObject->m_Adapter->Release();

        dataObject->m_BlendState->Release();
        dataObject->m_FrontCullRS->Release();
    }

    void ContextAPI::Create(ContextData* dataObject, GLFWwindow* windowHandle) {
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
            LARGE_INTEGER driverVersion;

            DXGI_ADAPTER_DESC adapterDescription;
            SecureZeroMemory(&adapterDescription, sizeof(DXGI_ADAPTER_DESC));

            dataObject->m_Adapter->GetDesc(&adapterDescription);
            wcstombs_s(NULL, videoCardDescription, 128, adapterDescription.Description, 128);

            dataObject->m_Adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);

            dataObject->m_ContextInfo.Vendor = DX11ContextAPIUtils::s_VendorCodeMap[adapterDescription.VendorId];
            dataObject->m_ContextInfo.Renderer = (Char*)Malloc(strlen(videoCardDescription));
            dataObject->m_ContextInfo.Renderer[strlen(videoCardDescription)] = '\0';

            Sentinel::MemFunctions::Memcpy(
                dataObject->m_ContextInfo.Renderer, videoCardDescription, strlen(videoCardDescription));
            dataObject->m_ContextInfo.API = "DirectX 11";

            Char buffer1[12] {}, buffer2[12] {}, buffer3[12] {}, buffer4[12] {};
            DX11ContextAPIUtils::ToString(buffer1, HIWORD(driverVersion.HighPart));
            DX11ContextAPIUtils::ToString(buffer2, LOWORD(driverVersion.HighPart));
            DX11ContextAPIUtils::ToString(buffer3, HIWORD(driverVersion.LowPart));
            DX11ContextAPIUtils::ToString(buffer4, LOWORD(driverVersion.LowPart));

            String major(buffer1), minor(buffer2), release(buffer3), build(buffer4), version;
            version = major + "." + minor + "." + release + "." + build;

            dataObject->m_ContextInfo.Version = (Char*)Malloc(version.Length());
            dataObject->m_ContextInfo.Version[version.Length()] = '\0';
            Sentinel::MemFunctions::Memcpy(dataObject->m_ContextInfo.Version, version.C_Str(), version.Length());

            // glfwMakeContextCurrent(windowHandle);

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

        {
            D3D11_RASTERIZER_DESC desc;
            SecureZeroMemory(&desc, sizeof(desc));
            desc.CullMode = D3D11_CULL_NONE;
            desc.FillMode = D3D11_FILL_SOLID;
            desc.FrontCounterClockwise = true;
            dataObject->m_Device->CreateRasterizerState(&desc, &dataObject->m_FrontCullRS);
        }

        dataObject->m_Context->RSSetState(dataObject->m_FrontCullRS);
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11