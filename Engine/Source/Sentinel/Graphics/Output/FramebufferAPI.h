#pragma once

#include "Sentinel/Memory/FixedSlabAllocator.h"
#include "Sentinel/Graphics/Output/FramebufferData.h"

#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DData.h"

#include <glm/fwd.hpp>

namespace Sentinel {
    class FramebufferAPI {
    public:
        static FramebufferData* CreateFramebufferData(
            FixedSlabAllocator<FramebufferData>& allocator,
            FixedSlabAllocator<RenderTexture2DData>& rtAllocator,
            FixedSlabAllocator<DepthTexture2DData>& dtAllocator,
            ContextData* context,
            UInt16 width,
            UInt16 height);

        static void Bind(FramebufferData* dataObject);

        static void Unbind(FramebufferData* dataObject);

        static void Clear(FramebufferData* dataObject, const glm::vec4& clearColor);

        static void Resize(FramebufferData* dataObject, const UInt16 width, const UInt16 height);

        static void Clean(FramebufferData* dataObject);

        inline static void SetAttachments(
            FramebufferData* dataObject,
            const std::initializer_list<ColorFormat>& attachments,
            const DepthFormat depthFormat = DepthFormat::NONE) {
            dataObject->m_ColorFormats = attachments;
            dataObject->m_DepthFormat = depthFormat;
        }

        inline static const ColorFormat GetColorFormat(FramebufferData* dataObject, UInt8 index) {
            return dataObject->m_ColorFormats[index];
        }

        inline static const DepthFormat GetDepthFormat(FramebufferData* dataObject) {
            return dataObject->m_DepthFormat;
        }

        inline static RenderTexture2DData* GetRenderTexture(FramebufferData* dataObject, UInt8 index) {
            return dataObject->m_RTAttachments[index];
        }

        inline static void SetRenderTexture(
            FramebufferData* dataObject, UInt8 index, RenderTexture2DData* renderTexture) {
            dataObject->m_RTAttachments[index] = renderTexture;
        }

        inline static const UInt32 GetWidth(FramebufferData* dataObject) { return dataObject->m_Width; }
        inline static const UInt32 GetHeight(FramebufferData* dataObject) { return dataObject->m_Height; }

        inline static void SetWidth(FramebufferData* dataObject, const UInt32 width) { dataObject->m_Width = width; }
        inline static void SetHeight(FramebufferData* dataObject, const UInt32 height) {
            dataObject->m_Height = height;
        }
    };
}  // namespace Sentinel