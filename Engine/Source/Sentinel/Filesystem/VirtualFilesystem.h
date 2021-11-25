#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Utilities/TSingleton.h"
#include "Sentinel/Filesystem/Filesystem.h"

namespace Sentinel
{
	class VirtualFilesystem : public TSingleton<VirtualFilesystem> {
	public:
		void Mount(const STL::string& virtualPath, const STL::string& physicalPath);
		void Unmount(const STL::string& path);
		Bool ResolvePhysicalPath(const STL::string& virtualPath, STL::string& outPhysicalPath);
		Bool AbsolutePathToVFS(const STL::string& absolutePath, STL::string& outVirtualPath);

		UInt8* ReadFile(const STL::string& path);
		STL::string ReadTextFile(const STL::string& path);

		Bool WriteToFile(const STL::string& path, UInt8* buffer);
		Bool WriteToTextFile(const STL::string& path, const STL::string& text);

	public:
		static void Init();
		static void Shutdown();

	private:
		STL::unordered_map<STL::string, STL::vector<STL::string>> m_MountPoints;
	};
}
