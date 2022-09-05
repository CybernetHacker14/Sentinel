#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Output/ViewportData.h"

namespace Sentinel {
    class GraphicsMemoryManager;

    class ViewportAPI {
    public:
        static ViewportData* CreateViewportData(
            SharedRef<GraphicsMemoryManager> memoryHandle,
            ContextData* context,
            UInt16 x,
            UInt16 y,
            UInt16 width,
            UInt16 height,
            UInt16 minDepth,
            UInt16 maxDepth);

        inline static void Bind(ViewportData* dataObject) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject);
        }

        inline static void Resize(ViewportData* dataObject, UInt16 width, UInt16 height) {
            if (!m_ResizeFunction) return;
            m_ResizeFunction(dataObject, width, height);
        }

    public:
        template<typename T>
        inline static T* Cast(ViewportData* dataObject) {
            static_assert(STL::is_base_of<ViewportData, T>::value, "'T' should be derived from ViewportData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(ViewportData*)> m_BindFunction;
        inline static STL::delegate<void(ViewportData*, UInt16, UInt16)> m_ResizeFunction;
    };
}  // namespace Sentinel