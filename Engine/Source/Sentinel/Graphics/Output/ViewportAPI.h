#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/Graphics/Output/ViewportData.h"

namespace Sentinel {
    class ViewportAPI {
    public:
        static ViewportData* CreateViewportData(
            PoolAllocator<ViewportData>& allocator,
            ContextData* context,
            UInt16 x,
            UInt16 y,
            UInt16 width,
            UInt16 height,
            UInt8 minDepth,
            UInt8 maxDepth);

        static void Bind(ViewportData* dataObject);

        static void Resize(ViewportData* dataObject, UInt16 width, UInt16 height);

    private:
#if ST_RENDERER_DX11
        static void Create(ViewportData* dataObject);
#endif  // ST_RENDERER_DX11
    };
}  // namespace Sentinel