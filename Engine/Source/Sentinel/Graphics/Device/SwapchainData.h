#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"

#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DData.h"

namespace Sentinel {
    struct SwapchainData {
    public:
        ContextData* Context;

    protected:
        Bool vSync;
        RenderTexture2DData* backbuffer;
        DepthTexture2DData* depthBuffer;

    private:
        friend class SwapchainAPI;
    };
}  // namespace Sentinel