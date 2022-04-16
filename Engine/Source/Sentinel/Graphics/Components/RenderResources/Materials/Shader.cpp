#include "stpch.h"
#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/Shader.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Materials/DX11Shader.h"

namespace Sentinel
{
	Shader::Shader(const STL::string& filepath, const STL::string& name)
		: m_FilePath(filepath), m_ShaderName(name) {}

	SharedRef<Shader> Shader::Create(const STL::string& filepath, const STL::string& name) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateSharedRef<DX11Shader>(filepath, name);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
