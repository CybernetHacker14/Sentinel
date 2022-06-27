#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Buffer/IndexbufferAPI.h"

namespace Sentinel {
    struct DX11IndexbufferData;

    class DX11IndexbufferAPI final: public IndexbufferAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_BindFunction = DX11IndexbufferAPI::Bind;
                    m_UnbindFunction = DX11IndexbufferAPI::Unbind;
                    m_CleanFunction = DX11IndexbufferAPI::Clean;
                }
            }
        } _initializer;

    private:
        static void Bind(IndexbufferData* dataObject);
        static void Unbind(IndexbufferData* dataObject);
        static void Clean(IndexbufferData* dataObject);

        static void CreateNative(DX11IndexbufferData* dataObject, void* indices, UInt32 count);

    private:
        friend class IndexbufferAPI;
    };
}  // namespace Sentinel