#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/System/UUID.h"

#include <flecs.h>

namespace Sentinel {
    struct Scene;

    struct Entity {
    public:
        Entity(Scene* scene, const STL::string& name);

        void SetParent(Entity* entity);
        Entity* GetParent();
        Bool IsParent();

        inline const flecs::entity& GetNative() { return m_Entity; }

    private:
        flecs::entity m_Entity;

        Scene* m_Scene = nullptr;
        Entity* m_Parent = nullptr;
    };
}  // namespace Sentinel
