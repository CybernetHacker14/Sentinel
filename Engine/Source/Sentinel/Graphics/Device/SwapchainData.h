#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"

namespace Sentinel {
    struct SwapchainData {
    public:
        ContextData* Context;

    protected:
        Bool vSync;

    private:
        friend class SwapchainAPI;
    };
}  // namespace Sentinel