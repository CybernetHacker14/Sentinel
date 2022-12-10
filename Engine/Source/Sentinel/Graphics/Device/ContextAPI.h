#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/Graphics/Device/ContextData.h"

struct GLFWwindow;

namespace Sentinel {
    class ContextAPI {
    public:
        static ContextData* CreateImmediateContext(PoolAllocator<ContextData>& allocator, GLFWwindow* windowHandle);

        static ContextData* CreateDeferredContext(PoolAllocator<ContextData>& allocator, GLFWwindow* windowHandle);

        static void Draw(ContextData* dataObject);

        static void DrawIndexed(ContextData* dataObject, const UInt32 count);

        static void Clean(ContextData* dataObject);

        inline static const ContextInfo& GetContextInfo(ContextData* dataObject) { return dataObject->m_ContextInfo; }
        inline static ContextType GetContextType(ContextData* dataObject) { return dataObject->m_ContextType; }

#ifdef ST_RENDERER_DX11
        inline static ID3D11Device* GetDevice(ContextData* databject) { return databject->m_Device; }
        inline static ID3D11DeviceContext* GetNativeContext(ContextData* dataObject) { return dataObject->m_Context; }
        inline static IDXGIDevice* GetDXGIDevice(ContextData* dataObject) { return dataObject->m_DXGIDevice; }
        inline static IDXGIAdapter* GetAdapter(ContextData* dataObject) { return dataObject->m_Adapter; }
        inline static IDXGIFactory* GetFactory(ContextData* dataObject) { return dataObject->m_Factory; }
#endif  // ST_RENDERER_DX11

    private:
        static void Create(ContextData* dataObject, GLFWwindow* windowHandle);
    };
}  // namespace Sentinel