#include "stpch.h"
#include "Sentinel/ECS/Scene.h"

#include "Sentinel/ECS/Components.h"

#include <cereal/types/string.hpp>

namespace Sentinel {
    Scene::Scene() {
        m_Scene.entity(flecs::World).set_doc_name("Untitled Scene");
    }

    Scene::~Scene() {
    }

    void Scene::SetName(CChar* name) {
        m_Scene.entity(flecs::World).set_doc_name(name);
    }

    CChar* Scene::GetName() const {
        return m_Scene.entity(flecs::World).doc_name();
    }

    Entity Scene::CreateEntity(CChar* name) {
        Entity entity = {&(m_Scene.entity(name)), this};
        entity.GetNative()->add<TransformComponent>();
        return entity;
    }

    void Scene::DeleteEntity(Entity* entity) {
        flecs::entity* e = entity->GetNative();
        e->destruct();
    }

    Entity Scene::FindEntityWithName(CChar* name) {
        Entity entity = {&(m_Scene.lookup(name)), this};
        return entity;
    }

    void Scene::SerializeToFile(CChar* path) {
        std::ofstream stream(path, std::ios::binary);
        {
            cereal::BinaryOutputArchive archive(stream);
            archive(cereal::make_nvp("Sentinel_Scene_File_1_0", *this));
        }
    }

    // TODO : Maybe oughta look for some resource loader functionality of some sorts
    void Scene::DeserializeFromFile(CChar* path) {
        std::ifstream stream(path, std::ifstream::in | std::ios::binary);
        cereal::BinaryInputArchive archive(stream);
        archive(cereal::make_nvp("Sentinel_Scene_File_1_0", *this));
    }

    std::stringstream Scene::SerializeToStream() {
        std::stringstream stream(std::ios::out | std::ios::in);
        {
            // https://github.com/USCiLab/cereal/issues/101

            cereal::BinaryOutputArchive archive(stream);
            archive(cereal::make_nvp("Sentinel_Scene_File_1_0", *this));
        }
        return stream;
    }

    // Maybe try passing STL::string here
    void Scene::DeserializeFromStream(std::stringstream& stream) {
        cereal::BinaryInputArchive archive(stream);
        archive(cereal::make_nvp("Sentinel_Scene_File_1_0", *this));
    }

    template<class Archive>
    inline void Sentinel::Scene::save(Archive& archive) const {
        archive(cereal::make_nvp(SCENE_UUID_KEY, (UUID)m_UUID));
        archive(cereal::make_nvp(SCENE_NAME_KEY, std::string(GetName())));
    }

    template<class Archive>
    void Sentinel::Scene::load(Archive& archive) {
        UInt64 uuid;
        std::string name;
        archive(cereal::make_nvp(SCENE_UUID_KEY, uuid));
        archive(cereal::make_nvp(SCENE_NAME_KEY, name));
        m_UUID = UUID(uuid);
        SetName(name.c_str());
    }
}  // namespace Sentinel
