#pragma once

#include "Sentinel/Common/Common.h"

#ifdef ST_RENDERER_DX11
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGIDevice;
struct IDXGIAdapter;
struct IDXGIFactory;

// TODO: Refactor
struct ID3D11BlendState;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    enum ContextType : Bool { IMMEDIATE = 0, DEFFERED = 1 };

    struct ContextInfo {
        STL::string Vendor;
        STL::string Renderer;
        STL::string API;
        STL::string Version;
    };

    struct ContextData {
    public:
        ContextInfo m_ContextInfo;
        ContextType m_ContextType;

    private:
#ifdef ST_RENDERER_DX11
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
        IDXGIDevice* m_DXGIDevice;
        IDXGIAdapter* m_Adapter;
        IDXGIFactory* m_Factory;

        ID3D11BlendState* m_BlendState;
#endif  // ST_RENDERER_DX11

        friend class ContextAPI;
    };
}  // namespace Sentinel