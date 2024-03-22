#include "stpch.h"
#include "_EXPORT/Filesystem/FilesystemAPI_EXPORT.h"
#include "Common/Core/DataTypes.h"

#include <Windows.h>
#include <wtypes.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include <PathCch.h>

void CALLBACK FileIOCompletionInfo(DWORD dwErrorCode, DWORD dwNoOfBytesTransferred, LPOVERLAPPED lpOverlapped) {
}

static HANDLE OpenFileForReading(CChar* path) {
    HANDLE value = CreateFileA(
        path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
    return value;
}

static LLong GetFileSizeInternal(const HANDLE file) {
    LARGE_INTEGER size;
    GetFileSizeEx(file, &size);
    return size.QuadPart;
}

static Int ReadFileInternal(const HANDLE file, void* buffer, const LLong size) {
    OVERLAPPED overlapped = {0};
#pragma warning(push, 0)
    return ReadFileEx(file, buffer, size, &overlapped, FileIOCompletionInfo);
#pragma warning(pop)
}

ST_API int Sentinel_FilesystemAPI_ReadTextFileAtPath(const char* filepath, char* buffer, unsigned long long* outSize) {
    Char* absolutePath = (Char*)Sentinel_Malloc(MAX_PATH + 1);
    DWORD length = GetFullPathNameA(filepath, MAX_PATH, absolutePath, 0);
    absolutePath[length] = '\0';

    const HANDLE file = OpenFileForReading(absolutePath);

    Sentinel_Free(absolutePath);

    if (file == INVALID_HANDLE_VALUE) return 0;
    *outSize = GetFileSizeInternal(file);

    const int result = ReadFileInternal(file, buffer, *outSize);
    CloseHandle(file);
    return result;
}
