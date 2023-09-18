#include "stpch.h"
#include "Sentinel/Filesystem/Filesystem.h"
#include "Sentinel/Common/Core/Assert.h"

#include <Windows.h>
#include <wtypes.h>
#include <shellapi.h>
#include <sstream>
#include <Shlwapi.h>
#include <PathCch.h>

namespace Sentinel {
    namespace WindowsFilesystemUtils {
        void CALLBACK FileIOCompletionInfo(DWORD dwErrorCode, DWORD dwNoOfBytesTransferred, LPOVERLAPPED lpOverlapped) {
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

        static HANDLE OpenFileForReading(const CChar* path) {
            return CreateFileA(
                path,
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

        static Bool IsDots(CChar* folderpath) {
            if (((folderpath[0] == '.') && (folderpath[1] == '\0')) ||
                ((folderpath[0] == '.') && (folderpath[1] == '.') && (folderpath[2] == '\0')))
                return true;

            return false;
        }

        static Bool DoesFolderExistInternal(CChar* path) {
            return PathFileExistsA(path) && PathIsDirectoryA(path);
        }

        static Bool RecursiveDeleteDirectory(CChar* path) {
            WIN32_FIND_DATAA findData;

            String searchMask(path);
            searchMask += "\\*";
            HANDLE handle =
                FindFirstFileExA(searchMask.C_Str(), FindExInfoBasic, &findData, FindExSearchNameMatch, nullptr, 0);

            if (handle == INVALID_HANDLE_VALUE) {
                FindClose(handle);
                return false;
            }

            do {
                const String virtualName(findData.cFileName);

                if (IsDots(virtualName.C_Str())) continue;

                Bool isDirectory = ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
                                   ((findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

                String filePath(path);
                filePath += '\\' + findData.cFileName;
                if (isDirectory) {
                    RecursiveDeleteDirectory(filePath.C_Str());
                } else {
                    Bool result = DeleteFileA(filePath.C_Str());
                }

            } while (FindNextFileA(handle, &findData) != 0);

            DWORD error = GetLastError();
            if (error != ERROR_NO_MORE_FILES) {
                ST_BREAKPOINT_ASSERT(false, "Error enumerating folder");
                return false;
            }
            FindClose(handle);
            return RemoveDirectoryA(path);
        }

    }  // namespace WindowsFilesystemUtils

    Path::Path(CChar* path) {
        Char buffer[MAX_PATH + 1];
        DWORD length = GetFullPathNameA(path, MAX_PATH, buffer, 0);
        buffer[length] = '\0';
        m_AbsolutePath = buffer;

        DWORD attributes = GetFileAttributesA(m_AbsolutePath);
        LPCSTR extension = PathFindExtensionA(m_AbsolutePath);

        m_Properties |= PathFileExistsA(m_AbsolutePath) ? ST_BIT(0) : m_Properties;
        m_Properties |= PathIsDirectoryA(m_AbsolutePath) ? ST_BIT(2) : ST_BIT(1);
        m_Properties |= attributes & FILE_ATTRIBUTE_READONLY ? ST_BIT(3) : m_Properties;
        m_Properties |= attributes & FILE_ATTRIBUTE_HIDDEN ? ST_BIT(4) : m_Properties;
        m_Properties |= PathIsURLA(m_AbsolutePath) ? ST_BIT(5) : m_Properties;
        m_Properties |= PathIsHTMLFileA(m_AbsolutePath) ? ST_BIT(6) : m_Properties;
        m_Properties |= UrlIsFileUrlA(m_AbsolutePath) ? ST_BIT(7) : m_Properties;
        m_Properties |= extension && extension[0] ? ST_BIT(8) : m_Properties;
        m_Properties |= (m_Properties & (ST_BIT(0))) && (m_Properties & (ST_BIT(1))) ? ST_BIT(9) : m_Properties;
        m_Properties |= (m_Properties & (ST_BIT(0))) && (m_Properties & (ST_BIT(2))) ? ST_BIT(10) : m_Properties;

        if (m_Properties & (ST_BIT(9))) {
            std::ifstream pFile(m_AbsolutePath);
            m_Properties |= pFile.peek() == std::ifstream::traits_type::eof() ? ST_BIT(11) : m_Properties;
        }

        m_Properties |=
            (m_Properties & (ST_BIT(10))) && PathIsDirectoryEmptyA(m_AbsolutePath) ? ST_BIT(12) : m_Properties;
    }

    CChar* Path::GetFilenameWithExtension() const {
        CChar* value = PathFindFileNameA(m_AbsolutePath);
        return value;
    }

    CChar* Path::GetFilenameWithoutExtension() const {
        Char* value = PathFindFileNameA(m_AbsolutePath);
        PathRemoveExtensionA(value);
        return value;
    }

    CChar* Path::GetExtension() const {
        CChar* value = PathFindExtensionA(PathFindFileNameA(m_AbsolutePath));
        return value;
    }

    void Filesystem::CreateFolder(const Path& folderpath) {
        if (folderpath.Exists() || folderpath.HasExtension()) return;
        CreateDirectoryA(folderpath.GetAbsolutePath(), nullptr);
    }

    Bool Filesystem::HasSubfolders(const Path& folderpath) {
        WIN32_FIND_DATAA findData;
        String path(folderpath.GetAbsolutePath());
        path += "\\*";
        HANDLE handle = FindFirstFileA(path.C_Str(), &findData);

        if (handle == INVALID_HANDLE_VALUE) {
            FindClose(handle);
            return false;
        }

        do {
            const String virtualName(findData.cFileName);

            if (WindowsFilesystemUtils::IsDots(virtualName.C_Str())) continue;

            if (WindowsFilesystemUtils::DoesFolderExistInternal((path + "\\" + virtualName).C_Str())) {
                FindClose(handle);
                return true;
            }

        } while (FindNextFileA(handle, &findData) != 0);
        FindClose(handle);

        return false;
    }

    Vector<Path> Filesystem::GetImmediateSubfolders(const Path& folderpath) {
        Vector<Path> subfolders;

        if (!HasSubfolders(folderpath)) return subfolders;

        WIN32_FIND_DATAA findData;
        String path(folderpath.GetAbsolutePath());
        path += "\\*";
        HANDLE handle = FindFirstFileA(path.C_Str(), &findData);

        if (handle == INVALID_HANDLE_VALUE) {
            FindClose(handle);
            return subfolders;
        }

        do {
            const String virtualName(findData.cFileName);

            if (WindowsFilesystemUtils::IsDots(virtualName.C_Str())) continue;

            const String subfolder(path + "\\" + virtualName);

            if (WindowsFilesystemUtils::DoesFolderExistInternal(subfolder.C_Str()))
                subfolders.Push_Back(subfolder.C_Str());

        } while (FindNextFileA(handle, &findData) != 0);

        FindClose(handle);

        return subfolders;
    }

    const Int64 Filesystem::GetFileSize(const Path& filepath) {
        const HANDLE file = WindowsFilesystemUtils::OpenFileForReading(filepath.GetAbsolutePath());
        if (file == INVALID_HANDLE_VALUE) return -1;
        return WindowsFilesystemUtils::GetFileSizeInternal(file);
    }

    Bool Filesystem::ReadFileAtPath(const Path& filepath, void* buffer, Int64& outSize) {
        const HANDLE file = WindowsFilesystemUtils::OpenFileForReading(filepath.GetAbsolutePath());
        if (file == INVALID_HANDLE_VALUE) return false;
        outSize = WindowsFilesystemUtils::GetFileSizeInternal(file);
        const Bool result = WindowsFilesystemUtils::ReadFileInternal(file, buffer, outSize);
        CloseHandle(file);
        return result;
    }

    Bool Filesystem::ReadTextFileAtPath(const Path& filepath, Char* buffer, Int64& outSize) {
        const HANDLE file = WindowsFilesystemUtils::OpenFileForReading(filepath.GetAbsolutePath());
        if (file == INVALID_HANDLE_VALUE) return false;
        outSize = WindowsFilesystemUtils::GetFileSizeInternal(file);
        const Bool result = WindowsFilesystemUtils::ReadFileInternal(file, buffer, outSize);
        CloseHandle(file);
        return result;
    }

    Bool Filesystem::WriteToFileAtPath(const Path& filepath, UInt8* buffer, UInt64 length) {
        const HANDLE file = CreateFileA(
            filepath.GetAbsolutePath(),
            GENERIC_WRITE | OPEN_EXISTING,
            0,
            NULL,
            filepath.DoesFileExist() ? OPEN_EXISTING : CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (file == INVALID_HANDLE_VALUE) {
            std::cout << WindowsFilesystemUtils::GetLastErrorAsString() << std::endl;
            return false;
        }

        const Int64 size = WindowsFilesystemUtils::GetFileSizeInternal(file);
        DWORD written;
        const Bool result = WriteFile(file, buffer, length, &written, nullptr) != 0;
        CloseHandle(file);
        return result;
    }

    Bool Filesystem::WriteToTextFileAtPath(const Path& filepath, const StringView& text) {
        return WriteToFileAtPath(filepath, (UInt8*)text.C_Str(), text.Size());  // Need to test the second parameter
    }

    Bool Filesystem::OpenAtPath(const Path& path) {
        if (!path.Exists()) return false;

        ShellExecuteA(nullptr, "open", path.GetAbsolutePath(), nullptr, nullptr, SW_SHOW);
        return true;
    }

    Bool Filesystem::DeleteAtPath(const Path& path) {
        if (path.DoesFileExist()) {
            return DeleteFileA(path.GetAbsolutePath());
        } else if (path.DoesFolderExist()) {
            return path.IsFolderEmpty() ? RemoveDirectoryA(path.GetAbsolutePath())
                                        : WindowsFilesystemUtils::RecursiveDeleteDirectory(path.GetAbsolutePath());
        }
        return false;
    }

    Bool Filesystem::MoveToPath(const Path& currentpath, const Path& newpath) {
        return MoveFileA(currentpath.GetAbsolutePath(), newpath.GetAbsolutePath());
    }

    Bool Filesystem::CopyToPath(const Path& currentpath, const Path& newpath) {
        return CopyFileA(currentpath.GetAbsolutePath(), newpath.GetAbsolutePath(), true);
    }

}  // namespace Sentinel
