#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/System/UUID.h"

#include <flecs.h>

namespace Sentinel {
    struct Entity {
    public:
        UUID uuid;
    };
}  // namespace Sentinel