#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

#include <flecs.h>

namespace Sentinel {
    struct Scene;

    struct Entity {
    public:
        Entity(flecs::entity* native, Scene* scene);

        CChar* GetName() const;
        void SetName(CChar* name);

        void SetParent(Entity* entity);
        Entity GetParent();
        Bool HasParent();
        Bool HasChildren();
        Int32 GetChildrenCount();

        inline flecs::entity* GetNative() { return m_Native; }
        inline Scene* GetScene() { return m_Scene; }

    private:
        Entity() = default;

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
