#pragma once

#include "Sentinel/Memory/FixedSlabAllocator.h"
#include "Sentinel/Graphics/Buffer/IndexbufferData.h"

namespace Sentinel {
    class IndexbufferAPI {
    public:
        static IndexbufferData* CreateIndexbufferData(
            FixedSlabAllocator<IndexbufferData>& allocator, ContextData* context, void* indices, UInt32 count);

        static void Bind(IndexbufferData* dataObject);

        static void Unbind(IndexbufferData* dataObject);

        static void Clean(IndexbufferData* dataObject);

        inline static const UInt32 GetCount(IndexbufferData* dataObject) { return dataObject->m_Count; }

    private:
        static void Create(IndexbufferData* dataObject, void* indices, UInt32 count);
    };
}  // namespace Sentinel