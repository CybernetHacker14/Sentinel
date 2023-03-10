#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/System/UUID.h"

#include <flecs.h>

namespace Sentinel {
    struct Scene;

    struct Entity {
    public:
        Entity(Scene* scene);

        void SetParent(Entity* entity);
        Entity* GetParent();
        Bool HasParent();
        Bool HasChildren();

        inline flecs::entity* GetNative() { return m_Native; }
        inline Scene* GetScene() { return m_Scene; }

    private:
        Entity() = default;

        void SetEntity(flecs::entity* native);

    private:
        flecs::entity* m_Native = nullptr;
        Scene* m_Scene = nullptr;

    private:
        friend struct Scene;
    };
}  // namespace Sentinel

namespace std {
    template<typename T>
    struct hash;

    template<>
    struct hash<flecs::entity> {
        Sentinel::Size_t operator()(const flecs::entity& entity) const { return (Sentinel::UInt64)entity; }
    };
}  // namespace std
