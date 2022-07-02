#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Buffer/VertexbufferAPI.h"

namespace Sentinel {
    struct DX11VertexbufferData;

    class DX11VertexbufferAPI final: public VertexbufferAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_BindFunction = DX11VertexbufferAPI::Bind;
                    m_UnbindFunction = DX11VertexbufferAPI::Unbind;
                    m_SetDataFunction = DX11VertexbufferAPI::SetData;
                    m_CleanFunction = DX11VertexbufferAPI::Clean;
                }
            }
        } _initializer;

    private:
        static void Bind(VertexbufferData* dataObject, UInt32 stride);
        static void Unbind(VertexbufferData* dataObject);
        static void SetData(VertexbufferData* dataObject, const void* vertices, UInt32 size);
        static void Clean(VertexbufferData* dataObject);

        static void CreateNative(DX11VertexbufferData* dataObject, UInt32 size);
        static void CreateNative(DX11VertexbufferData* dataObject, void* vertices, UInt32 size);

    private:
        friend class VertexbufferAPI;
    };
}  // namespace Sentinel