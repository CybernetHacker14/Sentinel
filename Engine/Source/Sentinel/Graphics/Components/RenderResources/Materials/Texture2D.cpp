#include "stpch.h"
#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/Texture2D.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Materials/DX11Texture2D.h"

namespace Sentinel
{
	Texture2D::Texture2D(const Texture2DImportSettings& settings)
		:m_Settings(settings) {}

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
