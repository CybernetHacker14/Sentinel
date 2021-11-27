#include "stpch.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

#include "Platform/DirectX11/Graphics/Components/Materials/DX11Shader.h"

namespace Sentinel
{
	void Shader::Bind() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Shader>()->Bind();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
		}
	}

	void Shader::Reload() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Shader>()->Reload();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
		}
	}

	const STL::string& Shader::GetShaderSource(const ShaderType& type) {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11Shader>()->GetShaderSource(type);
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return STL::string();
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return STL::string();
		}
	}

	const STL::string& Shader::GetName() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11Shader>()->GetName();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return STL::string();
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return STL::string();
		}
	}

	const std::filesystem::path& Shader::GetFilepath() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11Shader>()->GetFilepath();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return std::filesystem::path();
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return std::filesystem::path();
		}
	}

	SharedRef<Shader> Shader::Create(const std::filesystem::path& filepath, const STL::string& name) {
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
