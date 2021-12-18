#include "stpch.h"
#include "Sentinel/AssetManagement/AssetManager.h"
#include "Sentinel/Filesystem/Filesystem.h"
#include "Sentinel/AssetManagement/Asset.h"

namespace Sentinel
{
	AssetManager::AssetManager() {
		Bool value1 = Filesystem::HasExtensionInPath("TestDirectory1/EmptyFile.txt");
		Bool value2 = Filesystem::HasExtensionInPath("TestDirectory1");
		Bool value3 = Filesystem::DoesFileExist("TestDirectory/EmptyFile.txt");
		STL::string string1 = Filesystem::GetFilenameWithoutExtension("TestDirectory1/EmptyFile.txt");
		STL::string string2 = Filesystem::GetFilenameWithExtension("TestDirectory1/EmptyFile.txt");
		STL::string string3 = Filesystem::GetFilenameExtension("TestDirectory1/EmptyFile.txt");
		Filesystem::DeleteAtPath("TestDirectory1 - Copy/EmptyFile.txt");
		Filesystem::DeleteAtPath("TestDirectory1 - Copy");
	}
}
