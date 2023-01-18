#pragma once

#include "Sentinel/Common/Common.h"

#include <glm/glm.hpp>

namespace Sentinel {
    struct Transforms {
    public:
        STL::vector<glm::vec3> Positions;
        STL::vector<glm::vec3> Rotations;
        STL::vector<glm::vec3> Scales;
    };
}  // namespace Sentinel