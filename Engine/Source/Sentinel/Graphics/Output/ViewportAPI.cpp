#include "stpch.h"
#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"

#include "Sentinel/Graphics/Output/ViewportAPI.h"

#include "Platform/DirectX11/Graphics/Output/DX11ViewportAPI.h"

namespace Sentinel {
    ViewportData* ViewportAPI::CreateViewportData(
        SharedRef<GraphicsMemoryManager> memoryHandle,
        ContextData* context,
        UInt16 x,
        UInt16 y,
        UInt16 width,
        UInt16 height,
        UInt16 minDepth,
        UInt16 maxDepth) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                ViewportData* viewport = memoryHandle->ViewportAllocator.New<DX11ViewportData>();
                viewport->Context = context;
                viewport->x = x;
                viewport->y = y;
                viewport->width = width;
                viewport->height = height;
                viewport->depthMin = minDepth;
                viewport->depthMax = maxDepth;
                DX11ViewportAPI::Create(ViewportAPI::Cast<DX11ViewportData>(viewport));
                return viewport;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel