#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"
#include "Sentinel/Graphics/Material/ShaderData.h"

namespace Sentinel {
    enum class ColorFormat : UInt8 { NONE = 0, RGBA32F = 2, RGBA8UNORM = 28, R32SINT = 43 };

    enum class DepthFormat : UInt8 { NONE = 0, D24S8UINT = 45 };

    struct RenderTexture2DData {
    public:
        ContextData* Context;

    protected:
        void* m_PixelData;
        UInt32 m_Width, m_Height;
        UInt32 m_BindColorSlot;
        UInt32 m_BindDepthSlot;
        ColorFormat m_ColorFormat = ColorFormat::NONE;
        DepthFormat m_DepthFormat = DepthFormat::NONE;
        ShaderType m_BindColorType = ShaderType::NONE;
        ShaderType m_BindDepthType = ShaderType::NONE;

    private:
        friend class RenderTexture2DAPI;
    };
}  // namespace Sentinel