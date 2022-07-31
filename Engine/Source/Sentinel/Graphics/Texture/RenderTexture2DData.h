#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Material/ShaderData.h"

namespace Sentinel {
    enum class ColorFormat : UInt8 { NONE = 0, RGBA32F = 2, RGBA8UNORM = 28, R32SINT = 43 };

    struct RenderTexture2DData {
    public:
        ContextData* Context;
        SharedRef<GraphicsMemoryManager> MemoryHandle;

    protected:
        void* m_PixelData;
        UInt32 m_Width, m_Height;
        UInt32 m_BindSlot;
        ColorFormat m_Format = ColorFormat::NONE;
        ShaderType m_BindType = ShaderType::NONE;
        Bool m_SwapchainTarget = false;

    private:
        friend class RenderTexture2DAPI;
    };
}  // namespace Sentinel