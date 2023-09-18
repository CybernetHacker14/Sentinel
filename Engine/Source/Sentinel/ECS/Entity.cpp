#include "stpch.h"
#include "Sentinel/ECS/Entity.h"
#include "Sentinel/ECS/Scene.h"

#include "Sentinel/ECS/Components.h"

namespace Sentinel {
    Entity::Entity(flecs::entity* native, Scene* scene) : m_Native(native), m_Scene(scene) {
    }

    CChar* Entity::GetName() const {
        return m_Native->doc_name();
    }

    void Entity::SetName(CChar* name) {
        m_Native->set_doc_name(name);
    }

    void Entity::SetParent(Entity* entity) {
        m_Native->child_of(*(entity->m_Native));
    }

    Entity Entity::GetParent() {
        return Entity(&(m_Native->parent()), m_Scene);
    }

    Bool Entity::HasParent() {
        return &(m_Native->parent()) != nullptr;
    }

    Bool Entity::HasChildren() {
        auto f = m_Scene->GetNativeScene()->filter_builder().with(flecs::ChildOf, *m_Native).build();
        return f.is_true();
    }

    Int32 Entity::GetChildrenCount() {
        auto f = m_Scene->GetNativeScene()->filter_builder().with(flecs::ChildOf, *m_Native).build();
        return f.count();
    }

}  // namespace Sentinel
