#include "stpch.h"
#include "Sentinel/ECS/Entity.h"
#include "Sentinel/ECS/Scene.h"

#include "Sentinel/ECS/Components.h"

namespace Sentinel {
    Entity::Entity(Scene* scene) : m_Scene(scene) {
    }

    void Entity::SetParent(Entity* entity) {
        m_Native->child_of(*(entity->m_Native));
    }

    Entity* Entity::GetParent() {
        return m_Scene->registry[m_Native->parent()];
    }

    Bool Entity::HasParent() {
        return m_Scene->registry[m_Native->parent()] != nullptr;
    }

    Bool Entity::HasChildren() {
        auto f = m_Scene->GetNativeScene()->filter_builder().with(flecs::ChildOf, *m_Native).build();
        return f.is_true();
    }

    void Entity::SetEntity(flecs::entity* native) {
        m_Native = native;
        m_Native->add<TransformComponent>();
    }

}  // namespace Sentinel
