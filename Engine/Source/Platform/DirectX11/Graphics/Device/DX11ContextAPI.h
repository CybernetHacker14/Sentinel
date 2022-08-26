#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Device/ContextAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextData.h"

namespace Sentinel {
    class DX11ContextAPI final: public ContextAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_DrawFunction = DX11ContextAPI::Draw;
                    m_DrawIndexedFunction = DX11ContextAPI::DrawIndexed;
                }
            }
        } _initializer;

    public:
        inline static ID3D11Device* GetDevice(DX11ContextData* dataObject) { return dataObject->m_Device; }
        inline static ID3D11DeviceContext* GetNativeContext(DX11ContextData* dataObject) {
            return dataObject->m_Context;
        }
        inline static IDXGIDevice* GetDXGIDevice(DX11ContextData* dataObject) { return dataObject->m_DXGIDevice; }
        inline static IDXGIAdapter* GetAdapter(DX11ContextData* dataObject) { return dataObject->m_Adapter; }
        inline static IDXGIFactory* GetFactory(DX11ContextData* dataObject) { return dataObject->m_Factory; }

    private:
        static void Draw(ContextData* dataObject);
        static void DrawIndexed(ContextData* dataObject, const UInt32 count);

    private:
        static void Create(DX11ContextData* dataObject, GLFWwindow* windowHandle);

    private:
        friend class ContextAPI;
    };
}  // namespace Sentinel