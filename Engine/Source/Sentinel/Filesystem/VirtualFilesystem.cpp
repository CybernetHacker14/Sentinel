#include "stpch.h"
#include "Sentinel/Filesystem/VirtualFilesystem.h"
#include "Sentinel/Utilities/StringUtils.h"

namespace Sentinel
{
	void VirtualFilesystem::Mount(const STL::string& virtualPath, const STL::string& physicalPath) {
		m_MountPoints[virtualPath].emplace_back(physicalPath);
	}

	void VirtualFilesystem::Unmount(const STL::string& path) {
		m_MountPoints[path].clear();
	}

	Bool VirtualFilesystem::ResolvePhysicalPath(const STL::string& virtualPath, STL::string& outPhysicalPath) {
		if (!(virtualPath[0] == '/' && virtualPath[1] == '/'))
		{
			outPhysicalPath = virtualPath;
			return Filesystem::DoesPathExist(virtualPath);
		}

		// Break the path by '/' and get the list of directories of the path to search
		STL::vector<STL::string> directories = StringUtils::SplitString(virtualPath, "/");
		const STL::string& virtualDirectory = directories.front();

		// If it is the last directory it's the path itself
		if (m_MountPoints.find(virtualDirectory) == m_MountPoints.end() || m_MountPoints[virtualDirectory].empty())
		{
			outPhysicalPath = virtualPath;
			return Filesystem::DoesPathExist(virtualPath);
		}

		// Find the new path from the mount points using the virtual directories
		const STL::string& remainderString = virtualPath.substr(virtualDirectory.size() + 2,
			virtualPath.size() - virtualDirectory.size());

		for (const STL::string& physicalPath : m_MountPoints[virtualDirectory])
		{
			const STL::string newPath = physicalPath + remainderString;
			if (Filesystem::DoesPathExist(virtualPath))
			{
				outPhysicalPath = newPath;
				return true;
			}
		}

		return false;
	}

	Bool VirtualFilesystem::AbsolutePathToVFS(const STL::string& absolutePath, STL::string& outVirtualPath) {
		// Find the corresponding virtual path from the mount points using the complete file path
		for (auto const& [key, val] : m_MountPoints)
		{
			for (auto& vfsPath : val)
			{
				if (absolutePath.find(vfsPath) != STL::string::npos)
				{
					STL::string newPath = absolutePath;
					STL::string newPartPath = "//" + key;
					newPath.replace(0, vfsPath.length(), newPartPath);
					outVirtualPath = newPath;
					return true;
				}
			}
		}

		outVirtualPath = absolutePath;
		return false;
	}

	UInt8* VirtualFilesystem::ReadFile(const STL::string& path) {
		STL::string physicalPath;
		return ResolvePhysicalPath(path, physicalPath) ? Filesystem::ReadFileAtPath(path.c_str()) : nullptr;
	}

	STL::string VirtualFilesystem::ReadTextFile(const STL::string& path) {
		STL::string physicalPath;
		return ResolvePhysicalPath(path, physicalPath) ? Filesystem::ReadTextFileAtPath(path.c_str()) : STL::string();
	}

	Bool VirtualFilesystem::WriteToFile(const STL::string& path, UInt8* buffer) {
		STL::string physicalPath;
		return ResolvePhysicalPath(path, physicalPath) ? Filesystem::WriteToFileAtPath(path.c_str(), buffer) : false;
	}

	Bool VirtualFilesystem::WriteToTextFile(const STL::string& path, const STL::string& text) {
		STL::string physicalPath;
		return ResolvePhysicalPath(path, physicalPath) ? Filesystem::WriteToTextFileAtPath(path.c_str(), text) : false;
	}

	void VirtualFilesystem::Init() {}

	void VirtualFilesystem::Shutdown() {}
}
