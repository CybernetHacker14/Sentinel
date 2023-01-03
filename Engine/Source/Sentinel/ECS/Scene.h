#pragma once

#include <flecs.h>

namespace Sentinel {
    struct Scene {
    private:
        flecs::world m_NativeScene;
    };
}  // namespace Sentinel