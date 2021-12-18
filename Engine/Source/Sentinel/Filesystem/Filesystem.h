#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	class Filesystem {
	public:
		static STL::string GetAbsolutePath(const STL::string& path);

		static Bool DoesPathExist(const STL::string& path);

		static Bool IsFile(const STL::string& filepath);

		static Bool IsFolder(const STL::string& folderpath);

		/// <summary>
		/// Checks if the file exists at a path
		/// </summary>
		/// <param name="filepath"></param>
		/// <returns></returns>
		static Bool DoesFileExist(const STL::string& filepath);

		static Bool DoesFolderExist(const STL::string& folderpath);

		static Bool IsFolderEmpty(const STL::string& folderpath);

		static Bool HasSubFolders(const STL::string& folderpath);

		static Bool HasExtensionInPath(const STL::string& path);

		static STL::string GetFilenameWithoutExtension(const STL::string& path);

		static STL::string GetFilenameWithExtension(const STL::string& path);

		static STL::string GetFilenameExtension(const STL::string& path);

		static void CreateFolder(const STL::string& path);

		/// <summary>
		/// Reads a file at the path
		/// </summary>
		/// <param name="filepath">- Should be an existing file</param>
		/// <returns>A pointer to a buffer containing the data</returns>
		static UInt8* ReadFileAtPath(const STL::string& filepath);

		/// <summary>
		/// Reads a file at the path
		/// </summary>
		/// <param name="filepath">- Should be an existing file</param>
		/// <param name="buffer">- The buffer to which the file contents are read to</param>
		/// <param name="size">- The size of the file</param>
		/// <returns>Whether the operation is successful or not</returns>
		static Bool ReadFileAtPath(const STL::string& filepath, void* buffer, Int64 size = -1);

		/// <summary>
		/// Reads a text file at the path.
		/// </summary>
		/// <param name="filepath">- Should be an existing text file</param>
		/// <returns>A string containing the text data</returns>
		static STL::string ReadTextFileAtPath(const STL::string& filepath);

		/// <summary>
		/// Writes to the file at path.
		/// </summary>
		/// <param name="filepath">- Should be an existing file</param>
		/// <param name="buffer">A pointer to a buffer containing the data</param>
		/// <returns>Whether the operation is successful or not</returns>
		static Bool WriteToFileAtPath(const STL::string& filepath, UInt8* buffer);

		/// <summary>
		/// Writes to the text file at path
		/// </summary>
		/// <param name="filepath">- Should be an existing folder or file</param>
		/// <param name="text">- A string containing the text data</param>
		/// <returns>Whether the operation is successful or not</returns>
		static Bool WriteToTextFileAtPath(const STL::string& filepath, const STL::string& text);

		/// <summary>
		/// Opens a file or folder at path
		/// </summary>
		/// <param name="filepath">- Should be an existing folder or file</param>
		/// <returns>Whether the operation is successful or not</returns>
		static Bool OpenAtPath(const STL::string& filepath);

		/// <summary>
		/// Deletes a file or folder at path. The folder could be empty or non-empty
		/// </summary>
		/// <param name="filepath">- Should be an existing folder or file</param>
		/// <returns>Whether the operation is successful or not</returns>
		static Bool DeleteAtPath(const STL::string& filepath);

		/// <summary>
		/// Moves a file or folder from currentFilepath to newFilepath.
		/// </summary>
		/// <param name="currentFilepath">- Should be an existing folder or file</param>
		/// <param name="newFilepath">- Should be a folder only, 
		/// it shouldn't have any extensions like "Folder.old" or "Folder.bak"</param>
		/// <returns>Whether the operation is successful or not</returns>
		static Bool MoveToPath(const STL::string& currentFilepath, const STL::string& newFilepath);

		/// <summary>
		/// Copies a file or folder from currentFilepath to newFilepath.
		/// </summary>
		/// <param name="currentFilepath">- Should be an existing folder or file</param>
		/// <param name="newFilepath">- Should be a folder only, 
		/// it shouldn't have any extensions like "Folder.old" or "Folder.bak"</param>
		/// <returns>Whether the operation is successful or not</returns>
		static Bool CopyToPath(const STL::string& currentFilepath, const STL::string& newFilepath);
	};
}
