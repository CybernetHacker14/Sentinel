#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"

namespace Sentinel {
    class GraphicsMemoryManager;

    class ContextAPI {
    public:
        static ContextData* CreateImmediateContext(SharedRef<GraphicsMemoryManager> memoryHandle);
        static ContextData* CreateDeferredContext(SharedRef<GraphicsMemoryManager> memoryHandle);

        inline static const ContextInfo& GetContextInfo(ContextData* dataObject) { return dataObject->m_ContextInfo; }
        inline static ContextType GetContextType(ContextData* dataObject) { return dataObject->m_ContextType; }

    public:
        template<typename T>
        inline static T* Cast(ContextData* dataObject) {
            static_assert(STL::is_base_of<ContextData, T>::value, "'T' should be derived from ContextData.");
            return static_cast<T*>(dataObject);
        }
    };
}  // namespace Sentinel