#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Buffer/ConstantbufferData.h"

namespace Sentinel {
    class GraphicsMemoryManager;

    class ConstantbufferAPI {
    public:
        static ConstantbufferData* CreateConstantbufferData(
            SharedRef<GraphicsMemoryManager> memoryHandle, UInt32 size, UInt32 bindSlot, CBufferUsageType usageType);

        inline static void VSBind(ConstantbufferData* dataObject) {
            if (!m_VSBindFunction) return;
            m_VSBindFunction(dataObject);
        }

        inline static void PSBind(ConstantbufferData* dataObject) {
            if (!m_PSBindFunction) return;
            m_PSBindFunction(dataObject);
        }

        inline static void CSBind(ConstantbufferData* dataObject) {
            if (!m_CSBindFunction) return;
            m_CSBindFunction(dataObject);
        }

        inline static void SetStaticData(ConstantbufferData* dataObject, void* data) {
            if (!m_SetStaticDataFunction) return;
            m_SetStaticDataFunction(dataObject, data);
        }

        inline static void SetDynamicData(ConstantbufferData* dataObject, void* data) {
            if (!m_SetDynamicDataFunction) return;
            m_SetDynamicDataFunction(dataObject, data);
        }

        inline static void Clean(ConstantbufferData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

        inline static const void* GetData(ConstantbufferData* dataObject) { dataObject->m_Data; }
        inline static const UInt32 GetSize(ConstantbufferData* dataObject) { dataObject->m_Size; }
        inline static const CBufferUsageType GetUsageType(ConstantbufferData* dataObject) { dataObject->m_UsageType; }

    public:
        template<typename T>
        inline static T* Cast(ConstantbufferData* dataObject) {
            static_assert(
                STL::is_base_of<ConstantbufferData, T>::value, "'T' should be a derived from ConstantbufferData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(ConstantbufferData*)> m_VSBindFunction;
        inline static STL::delegate<void(ConstantbufferData*)> m_PSBindFunction;
        inline static STL::delegate<void(ConstantbufferData*)> m_CSBindFunction;

        inline static STL::delegate<void(ConstantbufferData*, void*)> m_SetStaticDataFunction;
        inline static STL::delegate<void(ConstantbufferData*, void*)> m_SetDynamicDataFunction;

        inline static STL::delegate<void(ConstantbufferData*)> m_CleanFunction;
    };
}  // namespace Sentinel