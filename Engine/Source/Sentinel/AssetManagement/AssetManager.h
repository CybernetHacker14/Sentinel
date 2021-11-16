#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/System/UUID.h"
#include "Sentinel/Filesystem/Filesystem.h"

namespace Sentinel
{
	class AssetManager {
	public:
		AssetManager();

	private:
		const std::filesystem::path m_AssetsRootNode = "Assets";
	};
}
