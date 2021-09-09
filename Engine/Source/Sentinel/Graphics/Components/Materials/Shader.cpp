#include "stpch.h"

#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

#include "Platform/DirectX11/Graphics/Components/Materials/DX11Shader.h"

namespace Sentinel
{
	Ref<ShaderBase> ShaderUtils::Create(const STL::string& filepath, const STL::string& name) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateRef<DX11Shader>(filepath, name);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
