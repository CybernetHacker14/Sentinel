#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/System/UUID.h"

#include "Sentinel/ECS/Entity.h"

#include <flecs.h>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Sentinel {
    struct Scene {
    public:
        Scene();
        ~Scene();

        void SetName(CChar* name);
        CChar* GetName() const;

        Entity CreateEntity(CChar* name = "New Entity");
        void DeleteEntity(Entity entity);

        Entity FindEntityWithName(CChar* name);

        void SerializeToFile(CChar* path);
        void DeserializeFromFile(CChar* path);

        std::stringstream SerializeToStream();
        void DeserializeFromStream(std::stringstream& stream);

    public:
        inline flecs::world* GetNativeScene() { return &m_Scene; }

    private:
        template<class Archive>
        void save(Archive& archive) const;

        template<class Archive>
        void load(Archive& archive);

    private:
        flecs::world m_Scene;
        UUID m_UUID {};

    private:
        static constexpr CChar* SCENE_UUID_KEY = "UUID";
        static constexpr CChar* SCENE_NAME_KEY = "Name";

    private:
        friend class cereal::access;
    };

}  // namespace Sentinel