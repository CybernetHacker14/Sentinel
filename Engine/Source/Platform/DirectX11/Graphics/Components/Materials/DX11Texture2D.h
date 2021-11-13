#pragma once

#include "Sentinel/Graphics/Components/Materials/Texture2D.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Texture2D : public Texture2DCRTP<DX11Texture2D> {
	public:
		DX11Texture2D();
		~DX11Texture2D();

	private:
		friend class Texture2DCRTP<DX11Texture2D>;
	};
}
