#include "stpch.h"

#include "Sentinel/Archive/ZipFileOperations.h"
#include "Sentinel/Filesystem/Filesystem.h"

#include <mz_strm.h>
#include <mz_zip.h>
#include <mz_zip_rw.h>
#include <mz_compat.h>

namespace Sentinel {
    Bool ZipFileOperations::DoesFileExistInZip(const STL::string& zipPath, const STL::string& inZipLocation) {
        unzFile zf = unzOpen64(zipPath.c_str());

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
        const STL::string& zipPath, const STL::string& inZipLocation, void** ptrToBuffer, UInt32& dataLength) {
        unzFile zf = unzOpen64(zipPath.c_str());

        if (zf == NULL) {
            ST_ASSERT(false, "Error opening zip file");
            return false;
        }

        if (unzGoToFirstFile(zf) == UNZ_OK) {
            do {
                if (unzOpenCurrentFile(zf) == UNZ_OK) {
                    unz_file_info64 fileInfo;
                    memset(&fileInfo, 0, sizeof(unz_file_info64));

                    if (unzGetCurrentFileInfo64(zf, &fileInfo, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK) {
                        char* filename = (char*)malloc(fileInfo.size_filename + 1);
                        unzGetCurrentFileInfo64(zf, &fileInfo, filename, fileInfo.size_filename + 1, NULL, 0, NULL, 0);
                        filename[fileInfo.size_filename] = '\0';
                        STL::string path(filename);

                        if (path == inZipLocation) {
                            *ptrToBuffer = new char[fileInfo.uncompressed_size];
                            unzReadCurrentFile(zf, *ptrToBuffer, fileInfo.uncompressed_size);
                            dataLength = fileInfo.uncompressed_size;
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

    Bool ZipFileOperations::WriteFileToZipFile(
        const STL::string& zipPath, const STL::string& inZipLocation, const STL::string& filepath) {
        std::fstream file(filepath.c_str(), std::ios::in);

        if (!file.is_open()) return false;

        Bool ret = false;
        if (file.is_open()) {
            file.seekg(0, std::ios::end);
            Long size = file.tellg();
            file.seekg(0, std::ios::beg);

            STL::vector<char> buffer(size);
            if (size == 0 || !file.read(&buffer[0], size)) {
                file.close();
                return ret;
            }

            ret = WriteBufferToZipFile(zipPath, inZipLocation, &buffer[0], size);
            file.close();
        }
        return ret;
    }

    Bool ZipFileOperations::WriteBufferToZipFile(
        const STL::string& zipPath, const STL::string& inZipLocation, std::stringstream stream) {
        stream.seekg(0, std::ios::end);
        Sentinel::UInt32 length = stream.tellg();
        stream.seekg(0, std::ios::beg);

        return WriteBufferToZipFile(zipPath, inZipLocation, stream.str().data(), length);
    }

    Bool ZipFileOperations::WriteBufferToZipFile(
        const STL::string& zipPath, const STL::string& inZipLocation, char* data, UInt32 length) {
        if (data == NULL || length == 0) return false;

        zipFile file = zipOpen64(
            zipPath.c_str(), !Filesystem::DoesFileExist(zipPath) ? APPEND_STATUS_CREATE : APPEND_STATUS_ADDINZIP);

        zip_fileinfo zfi = {0};
        int ret = zipOpenNewFileInZip_64(
            file,
            inZipLocation.c_str(),
            &zfi,
            NULL,
            0,
            NULL,
            0,
            "",
            MZ_COMPRESS_METHOD_STORE,
            MZ_COMPRESS_LEVEL_BEST,
            (length > 0xffffffff) ? 1 : 0);

        if (ret != MZ_OK) return false;

        ret = zipWriteInFileInZip(file, data, length);
        ret = zipCloseFileInZip(file);

        ret = zipClose_64(file, NULL);

        return ret == MZ_OK;
    }
}  // namespace Sentinel