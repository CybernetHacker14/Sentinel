#include "stpch.h"

#include "Sentinel/Archive/ZipFileOperations.h"
#include "Sentinel/Filesystem/Filesystem.h"

#include <mz.h>
#include <mz_compat.h>

namespace Sentinel {
    Bool ZipFileOperations::DoesFileExistInZip(const STL::string& zipPath, const STL::string& inZipLocation) {
        unzFile zf = unzOpen(zipPath.c_str());

        if (zf == NULL) { return false; }

        if (unzGoToFirstFile(zf) == UNZ_OK) {
            do {
                if (unzOpenCurrentFile(zf) == UNZ_OK) {
                    unz_file_info fileInfo = {0};

                    if (unzGetCurrentFileInfo(zf, &fileInfo, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK) {
                        char* filename = (char*)malloc(fileInfo.size_filename + 1);
                        unzGetCurrentFileInfo(zf, &fileInfo, filename, fileInfo.size_filename + 1, NULL, 0, NULL, 0);
                        filename[fileInfo.size_filename] = '\0';
                        STL::string path(filename);

                        if (path == inZipLocation) {
                            free(filename);
                            return true;
                        }

                        free(filename);
                    }

                    unzCloseCurrentFile(zf);
                }
            } while (unzGoToNextFile(zf) == UNZ_OK);
        }

        return false;
    }

    Bool ZipFileOperations::ReadFromZipFile(
        const STL::string& zipPath, const STL::string& inZipLocation, void** ptrToBuffer) {
        unzFile zf = unzOpen(zipPath.c_str());

        if (zf == NULL) {
            ST_ASSERT(false, "Error opening zip file");
            return false;
        }

        if (unzGoToFirstFile(zf) == UNZ_OK) {
            do {
                if (unzOpenCurrentFile(zf) == UNZ_OK) {
                    unz_file_info fileInfo;
                    memset(&fileInfo, 0, sizeof(unz_file_info));

                    if (unzGetCurrentFileInfo(zf, &fileInfo, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK) {
                        char* filename = (char*)malloc(fileInfo.size_filename + 1);
                        unzGetCurrentFileInfo(zf, &fileInfo, filename, fileInfo.size_filename + 1, NULL, 0, NULL, 0);
                        filename[fileInfo.size_filename] = '\0';
                        STL::string path(filename);

                        if (path == inZipLocation) {
                            *ptrToBuffer = new char[fileInfo.uncompressed_size];
                            unzReadCurrentFile(zf, *ptrToBuffer, fileInfo.uncompressed_size);
                            free(filename);
                            return true;
                        }

                        free(filename);
                    }

                    unzCloseCurrentFile(zf);
                }
            } while (unzGoToNextFile(zf) == UNZ_OK);
        }

        return false;
    }

    Bool ZipFileOperations::WriteToZipFile(
        const STL::string& zipPath, const STL::string& inZipLocation, std::stringstream stream) {
        stream.seekg(0, std::ios::end);
        Sentinel::UInt32 length = stream.tellg();
        stream.seekg(0, std::ios::beg);

        return WriteToZipFile(zipPath, inZipLocation, stream.str().c_str(), length);
    }

    Bool ZipFileOperations::WriteToZipFile(
        const STL::string& zipPath, const STL::string& inZipLocation, const char* data, UInt32 length) {
        int appendMode;

        zipFile zf = zipOpen(
            zipPath.c_str(),
            !Filesystem::DoesFileExist(zipPath) || DoesFileExistInZip(zipPath, inZipLocation) ? APPEND_STATUS_CREATE
                                                                                             : APPEND_STATUS_ADDINZIP);

        if (zf == NULL) return false;

        zip_fileinfo zfi = {0};

        if (MZ_OK != zipOpenNewFileInZip(
                         zf,
                         inZipLocation.c_str(),
                         &zfi,
                         NULL,
                         0,
                         NULL,
                         0,
                         "",
                         MZ_COMPRESS_METHOD_STORE,   // - This parameter here, fucking POS
                         MZ_COMPRESS_LEVEL_BEST)) {  // Compression level (9=max)
            return false;
        }

        if (MZ_OK != zipWriteInFileInZip(zf, length == 0 ? "" : data, length)) return false;

        if (MZ_OK != zipCloseFileInZip(zf)) return false;

        if (zipClose(zf, NULL)) return true;

        return false;
    }
}  // namespace Sentinel