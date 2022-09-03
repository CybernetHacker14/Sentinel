#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h"

namespace Sentinel {
    class DX11VertexbufferLayoutAPI final: public VertexbufferLayoutAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_BindFunction = DX11VertexbufferLayoutAPI::Bind;
                    m_UnbindFunction = DX11VertexbufferLayoutAPI::Unbind;
                    m_CreateLayoutFunction = DX11VertexbufferLayoutAPI::CreateLayout;
                    m_CleanFunction = DX11VertexbufferLayoutAPI::Clean;
                }
            }
        } _initializer;

    private:
        static void CreateLayout(VertexbufferLayoutData* dataObject, ShaderData* shader);
        static void Bind(VertexbufferLayoutData* dataObject);
        static void Unbind(VertexbufferLayoutData* dataObject);
        static void Clean(VertexbufferLayoutData* dataObject);

    private:
        friend class VertexbufferLayoutAPI;
    };
}  // namespace Sentinel