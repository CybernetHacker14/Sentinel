#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"

#include "Sentinel/ECS/Scene.h"

namespace Sentinel {
    class SceneManager {
    public:
        static void LoadFromFile(const STL::string& filePath);
        static void SaveToFile(const STL::string& filePath, Scene* scene);
    };
}  // namespace Sentinel