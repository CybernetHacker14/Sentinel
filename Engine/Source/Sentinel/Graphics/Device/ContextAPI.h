#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"

struct GLFWwindow;

namespace Sentinel {
    class GraphicsMemoryManager;

    class ContextAPI {
    public:
        static ContextData* CreateImmediateContext(
            SharedRef<GraphicsMemoryManager> memoryHandle, GLFWwindow* windowHandle);
        static ContextData* CreateDeferredContext(
            SharedRef<GraphicsMemoryManager> memoryHandle, GLFWwindow* windowHandle);

        inline static void Draw(ContextData* dataObject) {
            if (!m_DrawFunction) return;
            m_DrawFunction(dataObject);
        }

        inline static void DrawIndexed(ContextData* dataObject, const UInt32 count) {
            if (!m_DrawIndexedFunction) return;
            m_DrawIndexedFunction(dataObject, count);
        }

        inline static void Clean(ContextData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

        inline static const ContextInfo& GetContextInfo(ContextData* dataObject) { return dataObject->m_ContextInfo; }
        inline static ContextType GetContextType(ContextData* dataObject) { return dataObject->m_ContextType; }

    public:
        template<typename T>
        inline static T* Cast(ContextData* dataObject) {
            static_assert(STL::is_base_of<ContextData, T>::value, "'T' should be derived from ContextData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(ContextData*)> m_DrawFunction;
        inline static STL::delegate<void(ContextData*, const UInt32)> m_DrawIndexedFunction;
        inline static STL::delegate<void(ContextData*)> m_CleanFunction;
    };
}  // namespace Sentinel