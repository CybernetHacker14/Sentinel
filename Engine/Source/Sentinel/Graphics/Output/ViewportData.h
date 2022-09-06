#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"

namespace Sentinel {
    struct ViewportData {
    public:
        ContextData* Context;

        // Since viewport will be probably accessed by many classes, even outside the rendering domain, keeping
        // fields public. Currently, the viewport will be a component of both Framebuffer and Swapchain
    public:
        Float x, y;
        Float width, height;
        Float depthMin, depthMax;

    private:
        friend class ViewportAPI;
    };
}  // namespace Sentinel