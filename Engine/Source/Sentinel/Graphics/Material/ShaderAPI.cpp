#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Material/ShaderAPI.h"

#include "Platform/DirectX11/Graphics/Material/DX11ShaderData.h"
#include "Platform/DirectX11/Graphics/Material/DX11ShaderAPI.h"

namespace Sentinel {
    ShaderData* ShaderAPI::CreateShaderData(
        SharedRef<GraphicsMemoryManager> memoryHandle, const STL::string& filepath, const STL::string& name) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                ShaderData* shaderObject = memoryHandle->ShaderAllocator.New<DX11ShaderData>();
                shaderObject->m_Filepath = filepath;
                shaderObject->m_ShaderName = name;
                DX11ShaderAPI::Load(ShaderAPI::Cast<DX11ShaderData>(shaderObject));
                return shaderObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel