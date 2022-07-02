#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Device/ContextAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextData.h"

namespace Sentinel {
    class DX11ContextAPI final: public ContextAPI {
    public:
        inline static ID3D11Device* GetDevice(DX11ContextData* dataObject) { return dataObject->m_Device; }
        inline static ID3D11DeviceContext* GetNativeContext(DX11ContextData* dataObject) {
            return dataObject->m_Context;
        }
        inline static IDXGIDevice* GetDXGIDevice(DX11ContextData* dataObject) { return dataObject->m_DXGIDevice; }
        inline static IDXGIAdapter* GetAdapter(DX11ContextData* dataObject) { return dataObject->m_Adapter; }

    private:
        static void Create(DX11ContextData* dataObject);

    private:
        friend class ContextAPI;
    };
}  // namespace Sentinel