#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/ShaderAPI.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Materials/DX11ShaderData.h"
#include "Platform/DirectX11/Graphics/Components/RenderResources/Materials/DX11ShaderAPI.h"

namespace Sentinel {
    ShaderData* ShaderAPI::CreateShaderData(const STL::string& filepath, const STL::string& name) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                ShaderData* shaderObject =
                    Application::Get().EngineMemoryManager->ShaderAllocator.New<DX11ShaderData>();
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