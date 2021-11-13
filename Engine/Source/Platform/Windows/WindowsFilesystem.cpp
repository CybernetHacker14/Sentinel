#include "stpch.h"

#include "Sentinel/Filesystem/Filesystem.h"

#include <Windows.h>
#include <wtypes.h>
#include <shellapi.h>

namespace Sentinel
{
	// Internal
	void CALLBACK FileIOCompletionInfo(DWORD dwErrorCode, DWORD dwNoOfBytesTransferred, LPOVERLAPPED lpOverlapped) {}

	static HANDLE OpenFileForReading(const std::filesystem::path& path) {
		return CreateFileA(path.string().c_str(), GENERIC_READ, FILE_SHARE_READ,
			nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	}

	static Int64 GetFileSizeInternal(const HANDLE file) {
		LARGE_INTEGER size;
		GetFileSizeEx(file, &size);
		return size.QuadPart;
	}

	static Bool ReadFileInternal(const HANDLE file, void* buffer, const Int64 size) {
		OVERLAPPED overlapped = { 0 };
		return ReadFileEx(file, buffer, size, &overlapped, FileIOCompletionInfo);
	}
	// \Internal

	UInt8* Filesystem::ReadFileAtPath(const std::filesystem::path& filepath) {
		const HANDLE file = OpenFileForReading(filepath);
		const Int64 size = GetFileSizeInternal(file);
		UInt8* buffer = new UInt8[static_cast<UInt32>(size)];
		const Bool result = ReadFileInternal(file, buffer, size);
		CloseHandle(file);
		if (!result)
			delete[] buffer;

		return result ? buffer : nullptr;
	}

	Bool Filesystem::ReadFileAtPath(const std::filesystem::path& filepath, void* buffer, Int64 size) {
		const HANDLE file = OpenFileForReading(filepath);
		if (file == INVALID_HANDLE_VALUE)
			return false;

		if (size < 0)
			size = GetFileSizeInternal(file);

		Bool result = ReadFileInternal(file, buffer, size);
		CloseHandle(file);
		return result;
	}

	STL::string Filesystem::ReadTextFileAtPath(const std::filesystem::path& filepath) {
		const HANDLE file = OpenFileForReading(filepath);
		const Int64 size = GetFileSizeInternal(file);
		STL::string result(static_cast<UInt32>(size), 0);
		const Bool success = ReadFileInternal(file, &result[0], size);
		CloseHandle(file);

		if (success)
			result.erase(STL::remove(result.begin(), result.end(), '\r'), result.end());

		return success ? result : STL::string();
	}

	Bool Filesystem::WriteToFileAtPath(const std::filesystem::path& filepath, UInt8* buffer) {
		const HANDLE file = CreateFileA(filepath.string().c_str(), GENERIC_WRITE, NULL, nullptr, CREATE_NEW | OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, nullptr);

		if (file == INVALID_HANDLE_VALUE)
			return false;

		const Int64 size = GetFileSizeInternal(file);
		DWORD written;
		const Bool result = WriteFile(file, buffer, static_cast<DWORD>(size), &written, nullptr) != 0;
		CloseHandle(file);
		return result;
	}

	Bool Filesystem::WriteToTextFileAtPath(const std::filesystem::path& filepath, const STL::string& text) {
		return WriteToFileAtPath(filepath, (UInt8*)&text[0]);
	}

	Bool Filesystem::OpenAtPath(const std::filesystem::path& filepath) {
		if (!std::filesystem::exists(filepath))
			return false;

		ShellExecuteA(nullptr, "open", std::filesystem::absolute(filepath).string().c_str(), nullptr, nullptr, SW_SHOW);
		return true;
	}

	Bool Filesystem::DeleteAtPath(const std::filesystem::path& filepath) {
		if (!std::filesystem::exists(filepath))
			return false;

		return DeleteFileA(std::filesystem::absolute(filepath).string().c_str());
	}

	Bool Filesystem::MoveToPath(const std::filesystem::path& currentFilepath, const std::filesystem::path& newFilepath) {
		// currentFilepath valid examples			currentFilepath invalid examples
		// "TestDirectory1/TestDirectory2"			"TestDirectory1/TestDirectory2/" - Ending with path separator
		// "TestDirectory1/SomeFile.txt"			"TestDirectory/SomeFile.txt/" - Ending with path separator
		// 
		// newFilepath valid examples				newFilepath invalid examples
		// "TestDirectory2/TestDirectory3"			"TestDirectory/EmptyFile.txt"
		// 
		// Guard clauses
		if (!std::filesystem::exists(currentFilepath) || newFilepath.has_extension() ||
			*(currentFilepath.string().rbegin()) == '/' ||
			*(currentFilepath.string().rbegin()) == std::filesystem::path::preferred_separator ||
			*(newFilepath.string().rbegin()) == '/' ||
			*(newFilepath.string().rbegin()) == std::filesystem::path::preferred_separator)
			return false;

		if (!std::filesystem::exists(newFilepath))
			std::filesystem::create_directories(newFilepath);

		std::filesystem::path newPath = newFilepath;
		newPath += "/";
		newPath += currentFilepath.filename();

		if (std::filesystem::exists(newPath))
			return false;

		return MoveFileA(std::filesystem::absolute(currentFilepath).string().c_str(),
			std::filesystem::absolute(newPath).string().c_str());
	}

	Bool Filesystem::CopyToPath(const std::filesystem::path& currentFilepath, const std::filesystem::path& newFilepath) {
		// currentFilepath valid examples			currentFilepath invalid examples
		// "TestDirectory1/TestDirectory2"			"TestDirectory1/TestDirectory2/" - Ending with path separator
		// "TestDirectory1/SomeFile.txt"			"TestDirectory/SomeFile.txt/" - Ending with path separator
		// 
		// newFilepath valid examples				newFilepath invalid examples
		// "TestDirectory2/TestDirectory3"			"TestDirectory/EmptyFile.txt"
		// 
		// Guard clauses
		if (!std::filesystem::exists(currentFilepath) || newFilepath.has_extension() ||
			*(currentFilepath.string().rbegin()) == '/' ||
			*(currentFilepath.string().rbegin()) == std::filesystem::path::preferred_separator ||
			*(newFilepath.string().rbegin()) == '/' ||
			*(newFilepath.string().rbegin()) == std::filesystem::path::preferred_separator)
			return false;

		if (!std::filesystem::exists(newFilepath))
			std::filesystem::create_directories(newFilepath);

		std::filesystem::path newPath = newFilepath;
		newPath += "/";
		newPath += currentFilepath.filename();

		if (std::filesystem::exists(newPath))
			return false;

		return CopyFileA(std::filesystem::absolute(currentFilepath).string().c_str(),
			std::filesystem::absolute(newPath).string().c_str(), true);
	}
}
