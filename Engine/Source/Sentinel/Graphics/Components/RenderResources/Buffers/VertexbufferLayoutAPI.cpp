#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/VertexbufferLayoutAPI.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Buffers/DX11VertexbufferLayoutData.h"

namespace Sentinel {
    VertexbufferLayoutData* VertexbufferLayoutAPI::CreateVertexbufferLayoutData() {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                VertexbufferLayoutData* layoutObject =
                    Application::Get().EngineMemoryManager->LayoutAllocator.New<DX11VertexbufferLayoutData>();
                return layoutObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel