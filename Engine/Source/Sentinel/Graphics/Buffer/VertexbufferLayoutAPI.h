#pragma once

#include "Sentinel/Memory/FixedSlabAllocator.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutData.h"

namespace Sentinel {
    struct ShaderData;

    class VertexbufferLayoutAPI {
    public:
        static VertexbufferLayoutData* CreateVertexbufferLayoutData(
            FixedSlabAllocator<VertexbufferLayoutData>& allocator, ContextData* context);

        static void CreateLayout(VertexbufferLayoutData* dataObject, ShaderData* shader);

        static void Bind(VertexbufferLayoutData* dataObject);

        static void Unbind(VertexbufferLayoutData* dataObject);

        static void Clean(VertexbufferLayoutData* dataObject);

        inline static UInt32 GetStride(VertexbufferLayoutData* dataObject) { return dataObject->m_Stride; }
    };
}  // namespace Sentinel