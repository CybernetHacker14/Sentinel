#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Buffer/ConstantbufferAPI.h"

#include "Platform/DirectX11/Graphics/Buffer/DX11ConstantbufferData.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11ConstantbufferAPI.h"

namespace Sentinel {
    ConstantbufferData* ConstantbufferAPI::CreateConstantbufferData(
        SharedRef<GraphicsMemoryManager> memoryHandle, UInt32 size, UInt32 bindSlot, CBufferUsageType usageType) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                ConstantbufferData* bufferObject = memoryHandle->ConstantbufferAllocator.New<DX11ConstantbufferData>();
                DX11ConstantbufferAPI::CreateNative(
                    ConstantbufferAPI::Cast<DX11ConstantbufferData>(bufferObject), size, bindSlot, usageType);
                bufferObject->m_Size = size;
                bufferObject->m_UsageType = usageType;
                return bufferObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel