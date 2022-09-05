#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Output/DX11ViewportAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

namespace Sentinel {
    DX11ViewportAPI::_init DX11ViewportAPI::_initializer;

    void DX11ViewportAPI::Bind(ViewportData* dataObject) {
        DX11ViewportData* dxViewport = ViewportAPI::Cast<DX11ViewportData>(dataObject);
        DX11ContextData* dxContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* nativeContext = DX11ContextAPI::GetNativeContext(dxContext);

        nativeContext->RSSetViewports(1, &(dxViewport->m_Viewport));
    }

    void DX11ViewportAPI::Resize(ViewportData* dataObject, UInt16 width, UInt16 height) {
        dataObject->width = width;
        dataObject->height = height;
    }

    void DX11ViewportAPI::Create(DX11ViewportData* dataObject) {
        SecureZeroMemory(&(dataObject->m_Viewport), sizeof(dataObject->m_Viewport));

        dataObject->m_Viewport.TopLeftX = dataObject->x;
        dataObject->m_Viewport.TopLeftY = dataObject->y;
        dataObject->m_Viewport.Width = dataObject->width;
        dataObject->m_Viewport.Height = dataObject->height;
        dataObject->m_Viewport.MinDepth = dataObject->depthMin;
        dataObject->m_Viewport.MaxDepth = dataObject->depthMax;
    }
}  // namespace Sentinel