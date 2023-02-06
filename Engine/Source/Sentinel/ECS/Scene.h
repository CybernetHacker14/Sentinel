#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/System/UUID.h"

#include "Sentinel/ECS/Entity.h"

#include <flecs.h>

namespace Sentinel {
    struct Scene {
    public:
        Scene();
        ~Scene();

        Entity* CreateEntity(const STL::string& name = "New Entity");
        void DeleteEntity(Entity* entity);

        inline const UUID& GetUUID() { return m_UUID; }
        inline const flecs::world& GetNativeScene() { return m_Scene; }

    public:
        STL::vector<Entity*> entities;

    private:
        PoolAllocator<Entity> m_Allocator;
        UUID m_UUID {};
        flecs::world m_Scene;
        Entity* m_RootEntity;
    };
}  // namespace Sentinel
