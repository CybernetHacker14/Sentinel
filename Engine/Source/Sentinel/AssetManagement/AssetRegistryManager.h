#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/System/UUID.h"

namespace Sentinel
{
	class AssetRegistryManager : public ISharedRef {
	public:
		const STL::string& GetAssetPathForUUID(const UUID& uuid);

		void UpdateAssetPathForUUID(const UUID& uuid, const STL::string& filepath);
		void DeleteRegistryTuple(const UUID& uuid);

	private:
		void RegisterAllAssetsFromRoot();

	private:
		const STL::string& m_AssetRootPath = "assets";
		const STL::unordered_map<UUID, STL::string> m_AssetRegistry;
	};
}
