#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Buffer/IndexbufferData.h"

namespace Sentinel {
    class GraphicsMemoryManager;

    class IndexbufferAPI {
    public:
        static IndexbufferData* CreateIndexbufferData(
            SharedRef<GraphicsMemoryManager> memoryHandle, void* indices, UInt32 count);

        inline static void Bind(IndexbufferData* dataObject) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject);
        }

        inline static void Unbind(IndexbufferData* dataObject) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject);
        }

        inline static void Clean(IndexbufferData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

        inline static const UInt32 GetCount(IndexbufferData* dataObject) { return dataObject->m_Count; }

    public:
        template<typename T>
        inline static T* Cast(IndexbufferData* dataObject) {
            static_assert(STL::is_base_of<IndexbufferData, T>::value, "'T' should be a derived from IndexbufferData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(IndexbufferData*)> m_BindFunction;
        inline static STL::delegate<void(IndexbufferData*)> m_UnbindFunction;
        inline static STL::delegate<void(IndexbufferData*)> m_CleanFunction;
    };
}  // namespace Sentinel