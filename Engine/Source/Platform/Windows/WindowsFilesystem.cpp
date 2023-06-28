#include "stpch.h"
#include "Sentinel/Filesystem/Filesystem.h"
#include "Sentinel/Utilities/StringUtils.h"

#include "Platform/Windows/WindowsTextUtils.h"

#include <Windows.h>
#include <wtypes.h>
#include <shellapi.h>
#include <sstream>
#include <Shlwapi.h>
#include <PathCch.h>

namespace Sentinel {
    // Internal
    static const STL::wstring s_AllFilesMask(L"\\*");
    static const char s_SingleSlash('/');
    static const wchar_t s_DoubleSlash(L'\\');
    static const char* s_CDoubleSlash("\\");

    void CALLBACK FileIOCompletionInfo(DWORD dwErrorCode, DWORD dwNoOfBytesTransferred, LPOVERLAPPED lpOverlapped) {
    }

    static HANDLE OpenFileForReading(const STL::string& path) {
        return CreateFileW(
            WindowsTextUtils::TranscodeUTF8toUTF16(path).c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL);
    }

    static Int64 GetFileSizeInternal(const HANDLE file) {
        LARGE_INTEGER size;
        GetFileSizeEx(file, &size);
        return size.QuadPart;
    }

    static Bool ReadFileInternal(const HANDLE file, void* buffer, const Int64 size) {
        OVERLAPPED overlapped = {0};
#pragma warning(push, 0)
        return ReadFileEx(file, buffer, size, &overlapped, FileIOCompletionInfo);
#pragma warning(pop)
    }

    static std::string GetLastErrorAsString() {
        DWORD errorMessageID = GetLastError();
        if (errorMessageID == 0) return std::string();

        LPSTR messageBuffer = nullptr;

        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorMessageID,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&messageBuffer,
            0,
            NULL);

        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);
        return message;
    }

    static Bool IsDots(const STL::string& folderpath) {
        if (((folderpath[0] == '.') && (folderpath[1] == '\0')) ||
            ((folderpath[0] == '.') && (folderpath[1] == '.') && (folderpath[2] == '\0')))
            return true;

        return false;
    }

    static Bool IsDots(const STL::wstring& folderpath) {
        if (((folderpath[0] == '.') && (folderpath[1] == '\0')) ||
            ((folderpath[0] == '.') && (folderpath[1] == '.') && (folderpath[2] == '\0')))
            return true;

        return false;
    }

    static Bool RecursiveDeleteDirectory(const STL::wstring& path) {
        WIN32_FIND_DATAW findData;

        STL::wstring searchMask = path + s_AllFilesMask;
        HANDLE handle =
            FindFirstFileExW(searchMask.c_str(), FindExInfoBasic, &findData, FindExSearchNameMatch, nullptr, 0);

        if (handle == INVALID_HANDLE_VALUE) {
            FindClose(handle);
            return false;
        }

        do {
            const STL::wstring virtualName(findData.cFileName);

            if (IsDots(virtualName)) continue;

            Bool isDirectory = ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
                               ((findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

            STL::wstring filePath = path + L'\\' + findData.cFileName;
            if (isDirectory) {
                RecursiveDeleteDirectory(filePath);
            } else {
                Bool result = DeleteFileW(filePath.c_str());
            }

        } while (FindNextFileW(handle, &findData) != 0);

        DWORD error = GetLastError();
        if (error != ERROR_NO_MORE_FILES) {
            ST_ENGINE_ASSERT(false, "Error enumerating folder");
            return false;
        }
        FindClose(handle);
        return RemoveDirectoryW(path.c_str());
    }
    // \Internal

    STL::string Filesystem::GetAbsolutePath(const STL::string& path) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(path);
        wchar_t buffer[MAX_PATH + 1];
        DWORD length = GetFullPathNameW(wPath.c_str(), MAX_PATH, buffer, 0);
        buffer[length] = L'\0';
        return WindowsTextUtils::TranscodeUTF16toUTF8(buffer, length);
    }

    Bool Filesystem::DoesPathExist(const STL::string& path) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(path);
        return PathFileExistsW(wPath.c_str());
    }

    Bool Filesystem::IsFile(const STL::string& filepath) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(filepath);
        DWORD attributes = GetFileAttributesW(wPath.c_str());
        return (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
    }

    Bool Filesystem::IsFolder(const STL::string& folderpath) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(folderpath);
        DWORD attributes = GetFileAttributesW(wPath.c_str());
        return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }

    Bool Filesystem::DoesFileExist(const STL::string& filepath) {
        return DoesPathExist(filepath) && IsFile(filepath);
    }

    Bool Filesystem::DoesFolderExist(const STL::string& folderpath) {
        return DoesPathExist(folderpath) && IsFolder(folderpath);
    }

    void Filesystem::CreateFolder(const STL::string& folderpath) {
        if (DoesFolderExist(folderpath) || HasExtensionInPath(folderpath)) return;

        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(folderpath));
        CreateDirectoryW(wPath.c_str(), nullptr);
    }

    Bool Filesystem::IsFolderEmpty(const STL::string& folderpath) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(folderpath);
        return DoesFolderExist(folderpath) && PathIsDirectoryEmptyW(wPath.c_str());
    }

    Bool Filesystem::HasSubFolders(const STL::string& folderpath) {
        if (IsFolderEmpty(folderpath)) return false;

        WIN32_FIND_DATAW findData;
        const STL::string& absolutePath = GetAbsolutePath(folderpath);
        STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(folderpath);
        wPath += s_AllFilesMask;
        HANDLE handle = FindFirstFileW(wPath.c_str(), &findData);

        if (handle == INVALID_HANDLE_VALUE) {
            FindClose(handle);
            return false;
        }

        do {
            const STL::wstring virtualName(findData.cFileName);

            if (IsDots(virtualName)) continue;

            if (DoesFolderExist(absolutePath + s_CDoubleSlash + WindowsTextUtils::TranscodeUTF16toUTF8(virtualName))) {
                FindClose(handle);
                return true;
            }

        } while (FindNextFileW(handle, &findData) != 0);
        FindClose(handle);

        return false;
    }

    STL::vector<STL::string> Filesystem::GetImmediateSubfolders(const STL::string& folderpath) {
        STL::vector<STL::string> subfolders;

        if (!HasSubFolders(folderpath)) return subfolders;

        WIN32_FIND_DATAW findData;
        const STL::string& absolutePath = GetAbsolutePath(folderpath);
        STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(folderpath);
        wPath += s_AllFilesMask;
        HANDLE handle = FindFirstFileW(wPath.c_str(), &findData);

        if (handle == INVALID_HANDLE_VALUE) {
            FindClose(handle);
            return subfolders;
        }

        do {
            const STL::wstring virtualName(findData.cFileName);

            if (IsDots(virtualName)) continue;

            const STL::string subfolder(
                absolutePath + s_CDoubleSlash + WindowsTextUtils::TranscodeUTF16toUTF8(virtualName));

            if (DoesFolderExist(subfolder)) subfolders.emplace_back(subfolder);

        } while (FindNextFileW(handle, &findData) != 0);

        FindClose(handle);

        return subfolders;
    }

    STL::vector<STL::string> Filesystem::GetAllSubfolders(const STL::string& folderpath) {
        STL::vector<STL::string> subfolders;

        if (!HasSubFolders(folderpath)) return subfolders;

        WIN32_FIND_DATAW findData;
        const STL::string& absolutePath = GetAbsolutePath(folderpath);
        STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(folderpath);
        wPath += s_AllFilesMask;
        HANDLE handle = FindFirstFileW(wPath.c_str(), &findData);

        if (handle == INVALID_HANDLE_VALUE) {
            FindClose(handle);
            return subfolders;
        }

        do {
            const STL::wstring virtualName(findData.cFileName);

            if (IsDots(virtualName)) continue;

            const STL::string subfolder(
                absolutePath + s_CDoubleSlash + WindowsTextUtils::TranscodeUTF16toUTF8(virtualName));

            if (DoesFolderExist(subfolder)) subfolders.emplace_back(subfolder);

            if (HasSubFolders(subfolder)) {
                for (const auto& path: GetAllSubfolders(subfolder)) subfolders.emplace_back(path);
            }

        } while (FindNextFileW(handle, &findData) != 0);

        FindClose(handle);

        return subfolders;
    }

    Bool Filesystem::HasExtensionInPath(const STL::string& path) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(path);
        LPCWSTR extension = PathFindExtensionW(wPath.c_str());
        return extension && extension[0];
    }

    STL::string Filesystem::GetFilenameWithExtension(const STL::string& filepath) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(filepath);
        const STL::wstring& filename = STL::wstring(PathFindFileNameW(wPath.c_str()));
        return WindowsTextUtils::TranscodeUTF16toUTF8(filename);
    }

    STL::string Filesystem::GetFilenameWithoutExtension(const STL::string& filepath) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(filepath);
        PathRemoveExtensionW(PathFindFileNameW(wPath.c_str()));
        return WindowsTextUtils::TranscodeUTF16toUTF8(wPath.c_str());
    }

    STL::string Filesystem::GetFilenameExtension(const STL::string& filepath) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(filepath);
        const STL::wstring& extension = STL::wstring(PathFindExtensionW(PathFindFileNameW(wPath.c_str())));
        return WindowsTextUtils::TranscodeUTF16toUTF8(extension.c_str());
    }

    UInt8* Filesystem::ReadFileAtPath(const STL::string& filepath) {
        const HANDLE file = OpenFileForReading(filepath);
        const Int64 size = GetFileSizeInternal(file);
        UInt8* buffer = new UInt8[static_cast<UInt32>(size)];
        const Bool result = ReadFileInternal(file, buffer, size);
        CloseHandle(file);
        if (!result) delete[] buffer;

        return result ? buffer : nullptr;
    }

    Bool Filesystem::ReadFileAtPath(const STL::string& filepath, void* buffer, Int64 size) {
        const HANDLE file = OpenFileForReading(filepath);
        if (file == INVALID_HANDLE_VALUE) return false;

        if (size < 0) size = GetFileSizeInternal(file);

        Bool result = ReadFileInternal(file, buffer, size);
        CloseHandle(file);
        return result;
    }

    STL::string Filesystem::ReadTextFileAtPath(const STL::string& filepath) {
        const HANDLE file = OpenFileForReading(filepath);
        const Int64 size = GetFileSizeInternal(file);
        STL::string result(static_cast<UInt32>(size), 0);
        const Bool success = ReadFileInternal(file, &result[0], size);
        CloseHandle(file);

        if (success) result.erase(STL::remove(result.begin(), result.end(), '\r'), result.end());

        return success ? result : STL::string();
    }

    Bool Filesystem::WriteToFileAtPath(const STL::string& filepath, UInt8* buffer, UInt64 length) {
        const HANDLE file = CreateFileW(
            WindowsTextUtils::TranscodeUTF8toUTF16(filepath).c_str(),
            GENERIC_WRITE | OPEN_EXISTING,
            0,
            NULL,
            DoesFileExist(filepath) ? OPEN_EXISTING : CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (file == INVALID_HANDLE_VALUE) {
            std::cout << GetLastErrorAsString() << std::endl;
            return false;
        }

        const Int64 size = GetFileSizeInternal(file);
        DWORD written;
        const Bool result = WriteFile(file, buffer, length, &written, nullptr) != 0;
        CloseHandle(file);
        return result;
    }

    Bool Filesystem::WriteToTextFileAtPath(const STL::string& filepath, const STL::string& text) {
        return WriteToFileAtPath(filepath, (UInt8*)&text[0], text.size());
    }

    Bool Filesystem::OpenAtPath(const STL::string& path) {
        if (!DoesPathExist(path)) return false;

        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(path));
        ShellExecuteW(nullptr, L"open", wPath.c_str(), nullptr, nullptr, SW_SHOW);
        return true;
    }

    Bool Filesystem::DeleteAtPath(const STL::string& path) {
        const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(path));
        if (HasExtensionInPath(path)) {
            return DoesFileExist(path) ? DeleteFileW(wPath.c_str()) : false;
        } else if (DoesFolderExist(path)) {
            return IsFolderEmpty(path) ? RemoveDirectoryW(wPath.c_str()) : RecursiveDeleteDirectory(wPath);
        }
        return false;
    }

    Bool Filesystem::MoveToPath(const STL::string& currentPath, const STL::string& newPath) {
        // currentFilepath valid examples			currentFilepath invalid examples
        // "TestDirectory1/TestDirectory2"			"TestDirectory1/TestDirectory2/" - Ending with path
        // separator "TestDirectory1/SomeFile.txt"			"TestDirectory/SomeFile.txt/" - Ending with path
        // separator
        //
        // newFilepath valid examples				newFilepath invalid examples
        // "TestDirectory2/TestDirectory3"			"TestDirectory/EmptyFile.txt"
        //
        // Guard clauses
        auto currentPathEnding = *(currentPath.rbegin());
        auto newPathEnding = *(newPath.rbegin());
        if (!DoesPathExist(currentPath) || HasExtensionInPath(newPath) || currentPathEnding == s_SingleSlash ||
            currentPathEnding == s_DoubleSlash || newPathEnding == s_SingleSlash || newPathEnding == s_DoubleSlash) {
            return false;
        }

        if (!DoesFolderExist(newPath)) CreateFolder(newPath);

        STL::wstring& newFileName = WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(newPath));
        newFileName += s_DoubleSlash + WindowsTextUtils::TranscodeUTF8toUTF16(GetFilenameWithExtension(currentPath));

        return MoveFileW(
            WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(currentPath)).c_str(), newFileName.c_str());
    }

    Bool Filesystem::CopyToPath(const STL::string& currentPath, const STL::string& newPath) {
        // currentFilepath valid examples			currentFilepath invalid examples
        // "TestDirectory1/TestDirectory2"			"TestDirectory1/TestDirectory2/" - Ending with path
        // separator "TestDirectory1/SomeFile.txt"			"TestDirectory/SomeFile.txt/" - Ending with path
        // separator
        //
        // newFilepath valid examples				newFilepath invalid examples
        // "TestDirectory2/TestDirectory3"			"TestDirectory/EmptyFile.txt"
        //
        // Guard clauses
        auto currentPathEnding = *(currentPath.rbegin());
        auto newPathEnding = *(newPath.rbegin());
        if (!DoesPathExist(currentPath) || HasExtensionInPath(newPath) || currentPathEnding == s_SingleSlash ||
            currentPathEnding == s_DoubleSlash || newPathEnding == s_SingleSlash || newPathEnding == s_DoubleSlash) {
            return false;
        }

        if (!DoesFolderExist(newPath)) CreateFolder(newPath);

        STL::wstring& newFileName = WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(newPath));
        newFileName += s_DoubleSlash + WindowsTextUtils::TranscodeUTF8toUTF16(GetFilenameWithExtension(currentPath));

        return CopyFileW(
            WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(currentPath)).c_str(), newFileName.c_str(), true);
    }

    Int64 Filesystem::GetFileSize(const STL::string& filepath) {
        const HANDLE file = OpenFileForReading(filepath);
        return GetFileSizeInternal(file);
    }
}  // namespace Sentinel