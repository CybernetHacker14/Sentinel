#pragma once

#include "Sentinel/Graphics/Device/ContextData.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DData.h"
#include "Sentinel/Common/Containers/Vector.h"

namespace Sentinel {
    struct FramebufferData {
    public:
        ContextData* Context;

    private:
        Vector<ColorFormat> m_ColorFormats;
        DepthFormat m_DepthFormat;
        RenderTexture2DData* m_RTAttachments[5];
        DepthTexture2DData* m_DTAttachment;
        UInt32 m_Width, m_Height;

    private:
        friend class FramebufferAPI;
    };
}  // namespace Sentinel