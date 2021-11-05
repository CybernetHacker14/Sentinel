#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/System/UUID.h"

#include <filesystem>

namespace Sentinel
{
	class Asset : public IntrusiveRefObject {
	public:
		Asset(const std::filesystem::path& filepath);

		inline const std::filesystem::path& GetAssetFilePath() const { return m_AssetFilepath; }
		inline const UUID& GetAssetUUID() const { return m_AssetUUID; }

	private:
		std::filesystem::path m_AssetFilepath;
		UUID m_AssetUUID;
	};
}
