#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    // Filesystem I/O functions.
    // WARNING: Avoid too much usage, and mostly aimed for editor functionality
    class Filesystem {
    public:
        static STL::string GetAbsolutePath(const STL::string& path);

        static Bool DoesPathExist(const STL::string& path);

        static Bool IsFile(const STL::string& filepath);

        static Bool IsFolder(const STL::string& folderpath);

        static Bool DoesFileExist(const STL::string& filepath);

        static Bool DoesFolderExist(const STL::string& folderpath);

        static void CreateFolder(const STL::string& folderpath);

        static Bool IsFolderEmpty(const STL::string& folderpath);

        static Bool HasSubFolders(const STL::string& folderpath);

        static STL::vector<STL::string> GetImmediateSubfolders(const STL::string& folderpath);

        static STL::vector<STL::string> GetAllSubfolders(const STL::string& folderpath);

        static Bool HasExtensionInPath(const STL::string& path);

        static STL::string GetFilenameWithoutExtension(const STL::string& filepath);

        static STL::string GetFilenameWithExtension(const STL::string& filepath);

        static STL::string GetFilenameExtension(const STL::string& filepath);

        static UInt8* ReadFileAtPath(const STL::string& filepath);

        static Bool ReadFileAtPath(const STL::string& filepath, void* buffer, Int64 size = -1);

        static STL::string ReadTextFileAtPath(const STL::string& filepath);

        static Bool WriteToFileAtPath(const STL::string& filepath, UInt8* buffer);

        static Bool WriteToTextFileAtPath(const STL::string& filepath, const STL::string& text);

        static Bool OpenAtPath(const STL::string& path);

        static Bool DeleteAtPath(const STL::string& path);

        static Bool MoveToPath(const STL::string& currentPath, const STL::string& newPath);

        static Bool CopyToPath(const STL::string& currentPath, const STL::string& newPath);

        static Int32 CreateZipFile(
            const STL::string& zipDestination,
            const STL::string& inzipDataDestination,
            const char* data,
            UInt32 length);
    };
}  // namespace Sentinel
