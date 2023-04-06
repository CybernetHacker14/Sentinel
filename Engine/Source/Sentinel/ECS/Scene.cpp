#include "stpch.h"
#include "Sentinel/ECS/Scene.h"

#include <cereal/types/string.hpp>

namespace Sentinel {
    Scene::Scene() {
        m_Allocator.AllocateMemoryBlock(255);
    }

    Scene::~Scene() {
        m_Allocator.DeleteAll();
        m_Allocator.DeallocateMemoryBlock();
    }

    void Scene::SetName(const STL::string& name) {
        m_Scene.entity(flecs::World).set_doc_name(name.c_str());
    }

    void Scene::SetName(const char* name) {
        m_Scene.entity(flecs::World).set_doc_name(name);
    }

    const char* Scene::GetName() const {
        return m_Scene.entity(flecs::World).doc_name();
    }

    Entity* Scene::CreateEntity(const STL::string& name) {
        // Heavy dependency on std::unordered_map class here
        auto pair = registry.emplace(m_Scene.entity(name.c_str()), m_Allocator.New(this));
        Entity* entity = pair.first->second;
        pair.first->second->SetEntity(&(const_cast<flecs::entity&>(pair.first->first)));
        return entity;
    }

    void Scene::DeleteEntity(Entity* entity) {
        flecs::entity* e = entity->GetNative();
        e->children([&](flecs::entity e) {
            m_Allocator.Delete(registry[e]);
            e.destruct();
            registry.erase(e);
        });
        e->destruct();
        registry.erase(*e);
        m_Allocator.Delete(entity);
    }

    void Scene::SerializeToFile(const STL::string& path) {
        std::ofstream stream(path.c_str(), std::ios::binary);
        {
            cereal::JSONOutputArchive archive(stream);
            archive(cereal::make_nvp("Sentinel_Scene_File_1_0", *this));
        }
    }

    void Scene::DeserializeFromFile(const STL::string& path) {
        std::ifstream stream(path.c_str(), std::ifstream::in | std::ios::binary);
        cereal::JSONInputArchive archive(stream);
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
        archive(cereal::make_nvp(SCENE_UUID_KEY, std::string(m_UUID.ToString().c_str())));
        archive(cereal::make_nvp(SCENE_NAME_KEY, std::string(GetName())));
    }

    template<class Archive>
    void Sentinel::Scene::load(Archive& archive) {
        std::string uuidstring;
        std::string name;
        archive(cereal::make_nvp(SCENE_UUID_KEY, uuidstring));
        archive(cereal::make_nvp(SCENE_NAME_KEY, name));
        m_UUID.FromString(uuidstring.c_str());
        SetName(name.c_str());
    }
}  // namespace Sentinel
