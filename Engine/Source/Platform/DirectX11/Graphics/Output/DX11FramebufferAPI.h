#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Output/FramebufferAPI.h"

#include "Platform/DirectX11/Graphics/Output/DX11FramebufferData.h"

namespace Sentinel {
    class DX11FramebufferAPI final: public FramebufferAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_BindFunction = DX11FramebufferAPI::Bind;
                    m_UnbindFunction = DX11FramebufferAPI::Unbind;
                    m_ClearFunction = DX11FramebufferAPI::Clear;
                    m_ResizeFunction = DX11FramebufferAPI::Resize;
                    m_CleanFunction = DX11FramebufferAPI::Clean;
                }
            }
        } _initializer;

    private:
        static void Invalidate(FramebufferData* dataObject, SharedRef<GraphicsMemoryManager> memoryHandle);

        static void Bind(FramebufferData* dataObject);

        static void Unbind(FramebufferData* dataObject);

        static void Clear(FramebufferData* dataObject, const glm::vec4& clearColor);

        static void Resize(FramebufferData* dataObject, const UInt32 width, const UInt32 height);

        static void Clean(FramebufferData* dataObject);

    private:
        friend class FramebufferAPI;
    };
}  // namespace Sentinel