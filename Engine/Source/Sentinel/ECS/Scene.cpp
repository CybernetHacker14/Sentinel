#include "stpch.h"
#include "Sentinel/ECS/Scene.h"

namespace Sentinel {
    Scene::Scene() {
        m_Allocator.AllocateMemoryBlock(255);
    }

    Scene::~Scene() {
        m_Allocator.DeleteAll();
        m_Allocator.DeallocateMemoryBlock();
    }

    Entity* Scene::CreateEntity(const STL::string& name) {
        Entity* entity = m_Allocator.New(this, name);
        entities.push_back(entity);
        return entity;
    }

    void Scene::DeleteEntity(Entity* entity) {
    }
}  // namespace Sentinel