#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/Graphics/Buffer/VertexbufferData.h"

namespace Sentinel {
    struct ContextData;

    class VertexbufferAPI {
    public:
        static VertexbufferData* CreateVertexbufferData(
            PoolAllocator<VertexbufferData>& allocator, ContextData* context, UInt32 size);
        static VertexbufferData* CreateVertexbufferData(
            PoolAllocator<VertexbufferData>& allocator, ContextData* context, void* vertices, UInt32 size);

        static void Bind(VertexbufferData* dataObject, UInt32 stride);

        static void Unbind(VertexbufferData* dataObject);

        static void SetData(VertexbufferData* dataObject, const void* vertices, UInt32 size);

        static void Clean(VertexbufferData* dataObject);

    private:
        static void Create(VertexbufferData* dataObject, UInt32 size);
        static void Create(VertexbufferData* dataObject, void* vertices, UInt32 size);
    };
}  // namespace Sentinel