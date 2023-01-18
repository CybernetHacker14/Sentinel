#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/System/UUID.h"

#include <flecs.h>

namespace Sentinel {
    struct Scene {
    public:
        Scene();

        inline const flecs::world& GetNativeScene() { return m_NativeScene; }
        inline const UUID& GetSceneUUID() { return m_SceneUUID; }

    private:
        UUID m_SceneUUID {};
        flecs::world m_NativeScene;
    };
}  // namespace Sentinel