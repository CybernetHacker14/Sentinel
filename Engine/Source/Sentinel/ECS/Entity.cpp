#include "stpch.h"
#include "Sentinel/ECS/Entity.h"
#include "Sentinel/ECS/Scene.h"

namespace Sentinel {
    Entity::Entity(Scene* scene, const STL::string& name) {
        m_Scene = scene;
        m_Entity = m_Scene->GetNativeScene().entity(name.c_str());
    }

    void Entity::SetParent(Entity* entity) {
        m_Entity.child_of(entity->m_Entity);
        entity->m_Parent = entity;
    }

    Entity* Entity::GetParent() {
        return m_Parent;
    }

    Bool Entity::IsParent() {
        return m_Parent != nullptr;
    }

}  // namespace Sentinel
