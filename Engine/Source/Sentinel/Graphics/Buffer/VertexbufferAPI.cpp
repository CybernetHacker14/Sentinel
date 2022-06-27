#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Buffer/VertexbufferAPI.h"

#include "Sentinel/Graphics/Buffer/VertexbufferData.h"

#include "Platform/DirectX11/Graphics/Buffer/DX11VertexbufferData.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11VertexbufferAPI.h"

namespace Sentinel {
    VertexbufferData* VertexbufferAPI::CreateVertexbufferData(
        SharedRef<GraphicsMemoryManager> memoryHandle, UInt32 size) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                VertexbufferData* bufferObject = memoryHandle->VertexbufferAllocator.New<DX11VertexbufferData>();
                DX11VertexbufferAPI::CreateNative(VertexbufferAPI::Cast<DX11VertexbufferData>(bufferObject), size);
                return bufferObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }

    VertexbufferData* VertexbufferAPI::CreateVertexbufferData(
        SharedRef<GraphicsMemoryManager> memoryHandle, void* vertices, UInt32 size) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                VertexbufferData* bufferObject = memoryHandle->VertexbufferAllocator.New<DX11VertexbufferData>();
                bufferObject->m_Vertices = vertices;
                DX11VertexbufferAPI::CreateNative(
                    VertexbufferAPI::Cast<DX11VertexbufferData>(bufferObject), vertices, size);
                return bufferObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel