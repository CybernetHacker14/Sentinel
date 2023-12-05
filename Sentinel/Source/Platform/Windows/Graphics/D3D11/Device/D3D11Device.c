#include "stpch.h"
#include "Graphics/D3D11/Device/D3D11Device.h"

#include "Graphics/Buffer/Indexbuffer.h"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>

#pragma warning(push)
#pragma warning(disable : 6011)

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

static ID3D11Device* device;
static ID3D11DeviceContext* context;
static IDXGIFactory* factory;

#ifdef ST_DEBUG
static ID3D11Debug* debug;
#endif  // ST_DEBUG

void Sentinel_D3D11Device_Init() {
    UINT flags = 0;

#ifdef ST_DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif  // ST_DEBUG

    D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0, D3D11_SDK_VERSION, &device, NULL, &context);

    CreateDXGIFactory(&IID_IDXGIFactory, (void**)&factory);

#ifdef ST_DEBUG
    ID3D11Device_QueryInterface(device, &IID_ID3D11Debug, (void**)&debug);
#endif  // ST_DEBUG
}

void Sentinel_D3D11Device_Deinit() {
    IDXGIFactory_Release(factory);
    ID3D11DeviceContext_ClearState(context);
    ID3D11DeviceContext_Flush(context);
    ID3D11DeviceContext_Release(context);

    ID3D11Device_Release(device);

#ifdef ST_DEBUG
    ID3D11Debug_ReportLiveDeviceObjects(debug, D3D11_RLDO_DETAIL);
    ID3D11Debug_Release(debug);
#endif  // ST_DEBUG
}

void Sentinel_D3D11Device_Draw() {
    ID3D11DeviceContext_DrawIndexed(context, Sentinel_Indexbuffer_GetRegisteredIndexbufferCount(), 0, 0);
}

ID3D11Device* Sentinel_D3D11Device_GetDevice() {
    return device;
}

ID3D11DeviceContext* Sentinel_D3D11Device_GetContext() {
    return context;
}

IDXGIFactory* Sentinel_D3D11Device_GetFactory() {
    return factory;
}

#pragma warning(pop)
