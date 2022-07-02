#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    struct SwapchainData {
    protected:
        Bool vSync;

    private:
        friend class SwapchainAPI;
    };
}  // namespace Sentinel