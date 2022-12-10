#include "stpch.h"

#if ST_RENDERER_DX11

    #include "Sentinel/Graphics/Output/ViewportAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel {
    ViewportData* Sentinel::ViewportAPI::CreateViewportData(
        PoolAllocator<ViewportData>& allocator,
        ContextData* context,
        UInt16 x,
        UInt16 y,
        UInt16 width,
        UInt16 height,
        UInt8 minDepth,
        UInt8 maxDepth) {
        ViewportData* viewport = allocator.New();
        viewport->Context = context;
        viewport->x = x;
        viewport->y = y;
        viewport->width = width;
        viewport->height = height;
        viewport->depthMin = minDepth;
        viewport->depthMax = maxDepth;
        Create(viewport);
        return viewport;
    }

    void ViewportAPI::Bind(ViewportData* dataObject) {
        ID3D11DeviceContext* nativeContext = ContextAPI::GetNativeContext(dataObject->Context);
        nativeContext->RSSetViewports(1, &(dataObject->m_Viewport));
    }

    void ViewportAPI::Resize(ViewportData* dataObject, UInt16 width, UInt16 height) {
        dataObject->width = width;
        dataObject->height = height;
        Create(dataObject);
    }

    void ViewportAPI::Create(ViewportData* dataObject) {
        SecureZeroMemory(&(dataObject->m_Viewport), sizeof(dataObject->m_Viewport));

        dataObject->m_Viewport.TopLeftX = dataObject->x;
        dataObject->m_Viewport.TopLeftY = dataObject->y;
        dataObject->m_Viewport.Width = dataObject->width;
        dataObject->m_Viewport.Height = dataObject->height;
        dataObject->m_Viewport.MinDepth = dataObject->depthMin;
        dataObject->m_Viewport.MaxDepth = dataObject->depthMax;
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11