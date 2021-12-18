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

#include <filesystem>

namespace Sentinel
{
	// Internal
	void CALLBACK FileIOCompletionInfo(DWORD dwErrorCode, DWORD dwNoOfBytesTransferred, LPOVERLAPPED lpOverlapped) {}

	static HANDLE OpenFileForReading(const STL::string& path) {
		return CreateFileW(WindowsTextUtils::TranscodeUTF8toUTF16(path).c_str(), GENERIC_READ, FILE_SHARE_READ,
			nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	}

	static Int64 GetFileSizeInternal(const HANDLE file) {
		LARGE_INTEGER size;
		GetFileSizeEx(file, &size);
		return size.QuadPart;
	}

	static Bool ReadFileInternal(const HANDLE file, void* buffer, const Int64 size) {
		OVERLAPPED overlapped = { 0 };
	#pragma warning(push, 0)
		return ReadFileEx(file, buffer, size, &overlapped, FileIOCompletionInfo);
	#pragma warning(pop)
	}

	static std::string GetLastErrorAsString() {
		DWORD errorMessageID = GetLastError();
		if (errorMessageID == 0)
			return std::string();

		LPSTR messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);
		return message;
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

	Bool Filesystem::IsFolderEmpty(const STL::string& folderpath) {
		const STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(folderpath);
		return DoesFolderExist(folderpath) && PathIsDirectoryEmptyW(wPath.c_str());
	}

	Bool Filesystem::HasSubFolders(const STL::string& folderpath) {
		if (!DoesPathExist(folderpath))
			return false;

		WIN32_FIND_DATAW findData;
		STL::string& absolutePath = GetAbsolutePath(folderpath);
		STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(folderpath);
		wPath += L"\\*";
		HANDLE handle = FindFirstFileW(wPath.c_str(), &findData);

		if (handle == INVALID_HANDLE_VALUE)
		{
			FindClose(handle);
			return false;
		}

		do
		{
			const STL::wstring virtualName(findData.cFileName);

			if (((virtualName[0] == '.') && (virtualName[1] == '\0')) ||
				((virtualName[0] == '.') && (virtualName[1] == '.') &&
					(virtualName[2] == '\0')))
				continue;

			if (DoesFolderExist(absolutePath + "\\" + WindowsTextUtils::TranscodeUTF16toUTF8(virtualName)))
			{
				FindClose(handle);
				return true;
			}

		} while (FindNextFileW(handle, &findData) != 0);
		FindClose(handle);

		return false;
	}

	Bool Filesystem::HasExtensionInPath(const STL::string& path) {
		STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(path);
		LPCWSTR extension = PathFindExtensionW(wPath.c_str());
		return extension && extension[0];
	}

	STL::string Filesystem::GetFilenameWithExtension(const STL::string& path) {
		STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(path);
		STL::wstring& filename = STL::wstring(PathFindFileNameW(wPath.c_str()));
		return WindowsTextUtils::TranscodeUTF16toUTF8(filename);
	}

	STL::string Filesystem::GetFilenameWithoutExtension(const STL::string& path) {
		STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(path);
		PathRemoveExtensionW(PathFindFileNameW(wPath.c_str()));
		return WindowsTextUtils::TranscodeUTF16toUTF8(wPath.c_str());
	}

	STL::string Filesystem::GetFilenameExtension(const STL::string& path) {
		STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(path);
		STL::wstring& extension = STL::wstring(PathFindExtensionW(PathFindFileNameW(wPath.c_str())));
		return WindowsTextUtils::TranscodeUTF16toUTF8(extension.c_str());
	}

	void Filesystem::CreateFolder(const STL::string& path) {
		if (DoesFolderExist(path) || HasExtensionInPath(path))
			return;

		STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(path));
		CreateDirectoryW(wPath.c_str(), nullptr);
	}

	UInt8* Filesystem::ReadFileAtPath(const STL::string& filepath) {
		const HANDLE file = OpenFileForReading(filepath);
		const Int64 size = GetFileSizeInternal(file);
		UInt8* buffer = new UInt8[static_cast<UInt32>(size)];
		const Bool result = ReadFileInternal(file, buffer, size);
		CloseHandle(file);
		if (!result)
			delete[] buffer;

		return result ? buffer : nullptr;
	}

	Bool Filesystem::ReadFileAtPath(const STL::string& filepath, void* buffer, Int64 size) {
		const HANDLE file = OpenFileForReading(filepath);
		if (file == INVALID_HANDLE_VALUE)
			return false;

		if (size < 0)
			size = GetFileSizeInternal(file);

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

		if (success)
			result.erase(STL::remove(result.begin(), result.end(), '\r'), result.end());

		return success ? result : STL::string();
	}

	Bool Filesystem::WriteToFileAtPath(const STL::string& filepath, UInt8* buffer) {
		const HANDLE file = CreateFileW(WindowsTextUtils::TranscodeUTF8toUTF16(filepath).c_str(),
			GENERIC_WRITE, NULL, nullptr, CREATE_NEW | OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (file == INVALID_HANDLE_VALUE)
			return false;

		const Int64 size = GetFileSizeInternal(file);
		DWORD written;
		const Bool result = WriteFile(file, buffer, static_cast<DWORD>(size), &written, nullptr) != 0;
		CloseHandle(file);
		return result;
	}

	Bool Filesystem::WriteToTextFileAtPath(const STL::string& filepath, const STL::string& text) {
		return WriteToFileAtPath(filepath, (UInt8*)&text[0]);
	}

	Bool Filesystem::OpenAtPath(const STL::string& filepath) {
		if (!DoesPathExist(filepath))
			return false;

		STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(filepath));
		ShellExecuteW(nullptr, L"open", wPath.c_str(), nullptr, nullptr, SW_SHOW);
		return true;
	}

	Bool Filesystem::DeleteAtPath(const STL::string& filepath) {
		if (!DoesPathExist(filepath))
			return false;

		STL::wstring& wPath = WindowsTextUtils::TranscodeUTF8toUTF16(GetAbsolutePath(filepath));
		return DeleteFileW(wPath.c_str());
	}

	Bool Filesystem::MoveToPath(const STL::string& currentFilepath, const STL::string& newFilepath) {
		// currentFilepath valid examples			currentFilepath invalid examples
		// "TestDirectory1/TestDirectory2"			"TestDirectory1/TestDirectory2/" - Ending with path separator
		// "TestDirectory1/SomeFile.txt"			"TestDirectory/SomeFile.txt/" - Ending with path separator
		// 
		// newFilepath valid examples				newFilepath invalid examples
		// "TestDirectory2/TestDirectory3"			"TestDirectory/EmptyFile.txt"
		// 
		// Guard clauses
		/*if (!DoesPathExist(currentFilepath) || HasExtensionInPath(newFilepath) ||
			*(currentFilepath.rbegin()) == '/' ||
			*(currentFilepath.rbegin()) == std::filesystem::path::preferred_separator ||
			*(newFilepath.rbegin()) == '/' ||
			*(newFilepath.rbegin()) == std::filesystem::path::preferred_separator)
			return false;

		if (!DoesPathExist(newFilepath))
			std::filesystem::create_directories(newFilepath.c_str());

		std::filesystem::path newPath(newFilepath.c_str());
		newPath += "/";
		newPath += currentFilepath.filename();

		if (std::filesystem::exists(newPath))
			return false;

		return MoveFileA(std::filesystem::absolute(currentFilepath).string().c_str(),
			std::filesystem::absolute(newPath).string().c_str());*/
		return false;
	}

	Bool Filesystem::CopyToPath(const STL::string& currentFilepath, const STL::string& newFilepath) {
		// currentFilepath valid examples			currentFilepath invalid examples
		// "TestDirectory1/TestDirectory2"			"TestDirectory1/TestDirectory2/" - Ending with path separator
		// "TestDirectory1/SomeFile.txt"			"TestDirectory/SomeFile.txt/" - Ending with path separator
		// 
		// newFilepath valid examples				newFilepath invalid examples
		// "TestDirectory2/TestDirectory3"			"TestDirectory/EmptyFile.txt"
		// 
		// Guard clauses
		/*if (!std::filesystem::exists(currentFilepath) || HasExtensionInPath(newFilepath) ||
			*(currentFilepath.rbegin()) == '/' ||
			*(currentFilepath.rbegin()) == std::filesystem::path::preferred_separator ||
			*(newFilepath.rbegin()) == '/' ||
			*(newFilepath.rbegin()) == std::filesystem::path::preferred_separator)
			return false;

		if (!std::filesystem::exists(newFilepath))
			std::filesystem::create_directories(newFilepath);

		std::filesystem::path newPath = newFilepath;
		newPath += "/";
		newPath += currentFilepath.filename();

		if (std::filesystem::exists(newPath))
			return false;

		return CopyFileA(std::filesystem::absolute(currentFilepath).string().c_str(),
			std::filesystem::absolute(newPath).string().c_str(), true);*/
		return false;
	}
}
