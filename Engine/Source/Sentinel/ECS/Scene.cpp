#include "stpch.h"
#include "Sentinel/ECS/Scene.h"

namespace Sentinel {
    Scene::Scene(const STL::string& name) {
        m_Allocator.AllocateMemoryBlock(255);
    }

    Scene::~Scene() {
        m_Allocator.DeleteAll();
        m_Allocator.DeallocateMemoryBlock();
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

    void Scene::SerializeScene(const STL::string& path) {
        std::ofstream stream(path.c_str());
        cereal::JSONOutputArchive archive(stream);
        archive(*this);
    }
}  // namespace Sentinel
