#include "stpch.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Components/Materials/Texture2D.h"

#include "Platform/DirectX11/Graphics/Components/Materials/DX11Texture2D.h"

namespace Sentinel
{
	void Texture2D::Bind(UInt32 slot, const ShaderType shaderType) {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Texture2D>()->Bind(slot, shaderType);
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

	void Texture2D::Unbind(UInt32 slot, const ShaderType shaderType) {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Texture2D>()->Unbind(slot, shaderType);
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

	Bool Texture2D::IsHDR() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11Texture2D>()->IsHDR();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return false;
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return false;
		}
	}

	Bool Texture2D::IsLoaded() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11Texture2D>()->IsLoaded();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return false;
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return false;
		}
	}

	UInt32 Texture2D::GetWidth() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11Texture2D>()->GetWidth();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return 0;
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return 0;
		}
	}

	UInt32 Texture2D::GetHeight() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11Texture2D>()->GetHeight();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return 0;
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return 0;
		}
	}

	SharedRef<Texture2D> Texture2D::Create(const Texture2DImportSettings& settings) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateSharedRef<DX11Texture2D>(settings);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
