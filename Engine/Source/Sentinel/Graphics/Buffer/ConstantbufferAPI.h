#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/Graphics/Buffer/ConstantbufferData.h"

namespace Sentinel {
    class ConstantbufferAPI {
    public:
        static ConstantbufferData* CreateConstantbufferData(
            PoolAllocator<ConstantbufferData>& allocator,
            ContextData* context,
            UInt32 size,
            UInt32 bindSlot,
            CBufferUsageType usageType);

        static void VSBind(ConstantbufferData* dataObject);

        static void PSBind(ConstantbufferData* dataObject);

        static void CSBind(ConstantbufferData* dataObject);

        static void SetStaticData(ConstantbufferData* dataObject, void* data);

        static void SetDynamicData(ConstantbufferData* dataObject, void* data);

        static void Clean(ConstantbufferData* dataObject);

        inline static const void* GetData(ConstantbufferData* dataObject) { dataObject->m_Data; }
        inline static const UInt32 GetSize(ConstantbufferData* dataObject) { dataObject->m_Size; }
        inline static const CBufferUsageType GetUsageType(ConstantbufferData* dataObject) { dataObject->m_UsageType; }

    private:
        static void Create(ConstantbufferData* dataObject, UInt32 size, UInt32 bindSlot, CBufferUsageType usageType);
    };
}  // namespace Sentinel