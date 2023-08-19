#pragma once

#include "Sentinel/Common/Strings/StringView.h"
#include "Sentinel/Common/Containers/Vector.h"

namespace Sentinel {
    // Filesystem I/O functions.
    // WARNING: Avoid too much usage, and mostly aimed for editor functionality
    class Filesystem {
    public:
        static String GetAbsolutePath(const String& path);

        static Bool DoesPathExist(const String& path);

        static Bool IsFile(const String& filepath);

        static Bool IsFolder(const String& folderpath);

        static Bool DoesFileExist(const String& filepath);

        static Bool DoesFolderExist(const String& folderpath);

        static void CreateFolder(const String& folderpath);

        static Bool IsFolderEmpty(const String& folderpath);

        static Bool HasSubFolders(const String& folderpath);

        static Vector<String> GetImmediateSubfolders(const String& folderpath);

        static Vector<String> GetAllSubfolders(const String& folderpath);

        static Bool HasExtensionInPath(const String& path);

        static String GetFilenameWithoutExtension(const String& filepath);

        static String GetFilenameWithExtension(const String& filepath);

        static String GetFilenameExtension(const String& filepath);

        static UInt8* ReadFileAtPath(const String& filepath);

        static Bool ReadFileAtPath(const String& filepath, void* buffer, Int64 size = -1);

        static String ReadTextFileAtPath(const String& filepath);

        static Bool WriteToFileAtPath(const String& filepath, UInt8* buffer, UInt64 length);

        static Bool WriteToTextFileAtPath(const String& filepath, const String& text);

        static Bool OpenAtPath(const String& path);

        static Bool DeleteAtPath(const String& path);

        static Bool MoveToPath(const String& currentPath, const String& newPath);

        static Bool CopyToPath(const String& currentPath, const String& newPath);

        static Int64 GetFileSize(const String& filepath);
    };
}  // namespace Sentinel
