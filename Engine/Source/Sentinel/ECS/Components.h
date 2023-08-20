#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

#include <glm/glm.hpp>

namespace Sentinel {
    // Just a shell
    struct Component {};

    struct TransformComponent: public Component {
    public:
        glm::vec3 Position {0, 0, 0};
        glm::vec3 Rotation {0, 0, 0};
        glm::vec3 Scale {0, 0, 0};
    };
}  // namespace Sentinel
