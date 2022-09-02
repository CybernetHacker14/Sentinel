#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutData.h"
#include "Sentinel/Graphics/Material/ShaderData.h"

namespace Sentinel {
    class GraphicsMemoryManager;

    class VertexbufferLayoutAPI {
    public:
        static VertexbufferLayoutData* CreateVertexbufferLayoutData(
            SharedRef<GraphicsMemoryManager> memoryHandle, ContextData* context);

        inline static void CreateLayout(VertexbufferLayoutData* dataObject, ShaderData* shader) {
            if (!m_CreateLayoutFunction) return;
            m_CreateLayoutFunction(dataObject, shader);
        }

        inline static void Bind(VertexbufferLayoutData* dataObject) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject);
        }

        inline static void Unbind(VertexbufferLayoutData* dataObject) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject);
        }

        inline static void Clean(VertexbufferLayoutData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

        inline static UInt32 GetStride(VertexbufferLayoutData* dataObject) { return dataObject->m_Stride; }

    public:
        template<typename T>
        inline static T* Cast(VertexbufferLayoutData* dataObject) {
            static_assert(
                STL::is_base_of<VertexbufferLayoutData, T>::value,
                "'T' should be derived from VertexbufferLayoutData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::function<void(VertexbufferLayoutData*, ShaderData*)> m_CreateLayoutFunction;
        inline static STL::function<void(VertexbufferLayoutData*)> m_BindFunction;
        inline static STL::function<void(VertexbufferLayoutData*)> m_UnbindFunction;
        inline static STL::function<void(VertexbufferLayoutData*)> m_CleanFunction;
    };
}  // namespace Sentinel