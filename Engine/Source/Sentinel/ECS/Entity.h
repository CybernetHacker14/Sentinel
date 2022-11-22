#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/System/UUID.h"

namespace Sentinel {
    struct Entity {
    public:
        UUID uuid;

        Entity* parent;

        STL::vector<Entity*> children;
    };
}  // namespace Sentinel