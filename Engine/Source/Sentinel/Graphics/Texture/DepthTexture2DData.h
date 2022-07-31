#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Material/ShaderData.h"

namespace Sentinel {
    enum class DepthFormat : UInt8 { NONE = 0, D24S8UINT = 45 };

    struct DepthTexture2DData {
    public:
        ContextData* Context;
        SharedRef<GraphicsMemoryManager> MemoryHandle;

    protected:
        void* m_PixelData;
        UInt32 m_Width, m_Height;
        UInt32 m_BindSlot;
        DepthFormat m_Format = DepthFormat::NONE;
        ShaderType m_BindType = ShaderType::NONE;
        Bool m_SwapchainTarget = false;

    private:
        friend class DepthTexture2DAPI;
    };
}  // namespace Sentinel