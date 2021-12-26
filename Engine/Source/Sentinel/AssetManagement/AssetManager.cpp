#include "stpch.h"
#include "Sentinel/AssetManagement/AssetManager.h"
#include "Sentinel/Filesystem/Filesystem.h"
#include "Sentinel/AssetManagement/Asset.h"

namespace Sentinel
{
	AssetManager::AssetManager() {
		for (auto& path : Filesystem::GetAllSubfolders("TestDirectory1"))
		{
			ST_ENGINE_INFO("{0}", path.c_str());
		}
	}
}
