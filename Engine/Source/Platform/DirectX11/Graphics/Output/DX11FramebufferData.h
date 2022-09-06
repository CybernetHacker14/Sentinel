#pragma once

#include "Sentinel/Graphics/Output/FramebufferData.h"

namespace Sentinel {
    struct DX11FramebufferData final: public FramebufferData {
    private:
        friend class DX11FramebufferAPI;
    };
}  // namespace Sentinel