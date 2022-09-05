#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Output/ViewportAPI.h"

#include "Platform/DirectX11/Graphics/Output/DX11ViewportData.h"

namespace Sentinel {
    class DX11ViewportAPI final: public ViewportAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_BindFunction = DX11ViewportAPI::Bind;
                    m_ResizeFunction = DX11ViewportAPI::Resize;
                }
            }
        } _initializer;

    private:
        static void Bind(ViewportData* dataObject);
        static void Resize(ViewportData* dataObject, UInt16 width, UInt16 height);

    private:
        static void Create(DX11ViewportData* dataObject);

    private:
        friend class ViewportAPI;
    };
}  // namespace Sentinel