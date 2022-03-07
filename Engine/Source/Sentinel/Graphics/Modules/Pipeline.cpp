#include "stpch.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Modules/Pipeline.h"

#include "Platform/DirectX11/Graphics/Modules/DX11Pipeline.h"

namespace Sentinel
{
	void Pipeline::CreateInputLayout(SharedRef<Shader> shader) {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Pipeline>()->CreateInputLayout(shader);
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

	void Pipeline::Bind() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Pipeline>()->Bind();
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

	void Pipeline::Unbind() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Pipeline>()->Unbind();
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

	void Pipeline::Clean() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Pipeline>()->Clean();
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

	UInt32 Pipeline::GetStride() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11Pipeline>()->GetStride();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return UInt32();
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return UInt32();
		}
	}

	SharedRef<Pipeline> Pipeline::Create() {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
			{
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			}
			case Backend::API::DirectX11:
			{
				return CreateSharedRef<DX11Pipeline>();
			}
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
