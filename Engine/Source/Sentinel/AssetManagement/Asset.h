#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/System/UUID.h"
#include "Sentinel/AssetManagement/AssetInfo.h"

namespace Sentinel
{
	class Asset : public ISharedRef {
	public:
		inline const STL::string& GetAssetFilePath() const { return m_AssetFilepath; }
		inline const UUID& GetAssetUUID() const { return m_AssetUUID; }
		inline const AssetType& GetAssetType() const { return m_AssetType; }

	private:
		STL::string m_AssetFilename;
		STL::string m_AssetFileExtension;
		STL::string m_AssetVirtualFilepath;

		UUID m_AssetUUID;
		AssetType m_AssetType;
	};
}
