#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class GraphicsMemoryManager;
    struct VertexbufferData;

    class VertexbufferAPI {
    public:
        static VertexbufferData* CreateVertexbufferData(SharedRef<GraphicsMemoryManager> memoryHandle, UInt32 size);
        static VertexbufferData* CreateVertexbufferData(
            SharedRef<GraphicsMemoryManager> memoryHandle, void* vertices, UInt32 size);

        inline static void Bind(VertexbufferData* dataObject, UInt32 stride) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject, stride);
        }

        inline static void Unbind(VertexbufferData* dataObject) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject);
        }

        inline static void SetData(VertexbufferData* dataObject, const void* vertices, UInt32 size) {
            if (!m_SetDataFunction) return;
            m_SetDataFunction(dataObject, vertices, size);
        }

        inline static void Clean(VertexbufferData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

    public:
        template<typename T>
        inline static T* Cast(VertexbufferData* dataObject) {
            static_assert(
                STL::is_base_of<VertexbufferData, T>::value, "'T' should be a derived from VertexbufferData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(VertexbufferData*, UInt32)> m_BindFunction;
        inline static STL::delegate<void(VertexbufferData*)> m_UnbindFunction;
        inline static STL::delegate<void(VertexbufferData*, const void*, UInt32)> m_SetDataFunction;
        inline static STL::delegate<void(VertexbufferData*)> m_CleanFunction;
    };
}  // namespace Sentinel