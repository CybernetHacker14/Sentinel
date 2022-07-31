#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DData.h"

namespace Sentinel {
    struct FramebufferData {
    public:
        inline FramebufferData() { m_ColorFormats.reserve(5); }

    public:
        ContextData* Context;

    protected:
        STL::vector<ColorFormat> m_ColorFormats;
        DepthFormat m_DepthFormat;
        RenderTexture2DData* m_RTAttachments[5];
        DepthTexture2DData* m_DTAttachment;
        UInt32 m_Width, m_Height;

    private:
        friend class FramebufferAPI;
    };
}  // namespace Sentinel