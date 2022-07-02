#pragma once

#include "Sentinel/Graphics/Device/ContextData.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGIDevice;
struct IDXGIAdapter;

namespace Sentinel {
    class DX11ContextData final: public ContextData {
    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
        IDXGIDevice* m_DXGIDevice;
        IDXGIAdapter* m_Adapter;

    private:
        friend class DX11ContextAPI;
    };
}  // namespace Sentinel