#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/System/UUID.h"

#include "Sentinel/ECS/Entity.h"

#include <flecs.h>
#include <cereal/archives/json.hpp>

namespace Sentinel {
    struct Scene {
    public:
        Scene();
        ~Scene();

        void SetName(const STL::string& name);
        void SetName(const char* name);
        const char* GetName() const;

        Entity* CreateEntity(const STL::string& name = "New Entity");
        void DeleteEntity(Entity* entity);

        void SerializeScene(const STL::string& path);
        void DeserializeScene(const STL::string& path);

    public:
        inline const UUID& GetUUID() { return m_UUID; }
        inline flecs::world* GetNativeScene() { return &m_Scene; }

    private:
        template<class Archive>
        void save(Archive& archive) const;

        template<class Archive>
        void load(Archive& archive);

    public:
        STL::unordered_map<flecs::entity, Entity*> registry;

    private:
        PoolAllocator<Entity> m_Allocator;
        UUID m_UUID {};
        flecs::world m_Scene;

    private:
        static constexpr const char* SCENE_UUID_KEY = "UUID";
        static constexpr const char* SCENE_NAME_KEY = "Name";

    private:
        friend class cereal::access;
    };

}  // namespace Sentinel