#include "stpch.h"

#include "Sentinel/Archive/ZipFileOperations.h"
#include "Sentinel/Common/Core/Malloc.h"
#include "Sentinel/Common/Strings/MemFunctions.h"

#include <mz_strm.h>
#include <mz_zip.h>
#include <mz_zip_rw.h>
#include <mz_compat.h>
#include <mz_strm_mem.h>
#include <mz_strm_buf.h>

#define HAVE_PKCRYPT

namespace Sentinel {

    static constexpr const char* s_ZipPassword = "password";
    static constexpr const UInt16 s_ZipCompressionLevel = MZ_COMPRESS_LEVEL_BEST;
    static constexpr const UInt16 s_ZipCompressionMethod = MZ_COMPRESS_METHOD_STORE;
    // If issue occurs check with Compression Method = MZ_COMPRESS_METHOD_STORE

    Bool ZipFileOperations::DoesFileExistInZip(const String& zipPath, const String& inZipLocation) {
        void* handle = NULL;
        Int32 err = MZ_OK, out = MZ_OK;

        mz_zip_reader_create(&handle);
        mz_zip_reader_set_password(handle, s_ZipPassword);

        err = mz_zip_reader_open_file(handle, zipPath.C_Str());
        out = mz_zip_reader_locate_entry(handle, inZipLocation.C_Str(), 0);
        err = mz_zip_reader_close(handle);

        return out == MZ_OK && err == MZ_OK;
    }

    Bool ZipFileOperations::ReadFromZipFile(
        const String& zipPath, const String& inZipLocation, void** ptrToBuffer, UInt32& dataLength) {
        void* handle = NULL;
        Int32 err = MZ_OK;

        mz_zip_reader_create(&handle);
        mz_zip_reader_set_password(handle, s_ZipPassword);

        err = mz_zip_reader_open_file(handle, zipPath.C_Str());

        if (mz_zip_reader_locate_entry(handle, inZipLocation.C_Str(), 0) == MZ_OK) {
            Int32 length = mz_zip_reader_entry_save_buffer_length(handle);
            *ptrToBuffer = new char[length];
            err = mz_zip_reader_entry_open(handle);
            err = mz_zip_reader_entry_read(handle, *ptrToBuffer, length);
            err = mz_zip_reader_entry_close(handle);
            dataLength = length;
        }

        err = mz_zip_reader_close(handle);
        return err == MZ_OK;
    }

    Bool ZipFileOperations::WriteTextFileToZipFile(
        const String& zipPath, const String& inZipLocation, const String& filepath) {
        std::fstream file(filepath.C_Str(), std::ios::in);

        if (!file.is_open()) return false;

        Bool ret = false;
        if (file.is_open()) {
            file.seekg(0, std::ios::end);
            Long size = file.tellg();
            file.seekg(0, std::ios::beg);

            Char* buffer = (Char*)Malloc(size);
            if (size == 0 || !file.read(buffer, size)) {
                file.close();
                return ret;
            }

            ret = WriteBufferToZipFile(zipPath, inZipLocation, buffer, size);
            file.close();
            Free(buffer);
        }
        return ret;
    }

    Bool ZipFileOperations::WriteBufferToZipFile(
        const String& zipPath, const String& inZipLocation, std::stringstream stream) {
        stream.seekg(0, std::ios::end);
        Sentinel::UInt32 length = stream.tellg();
        stream.seekg(0, std::ios::beg);

        return WriteBufferToZipFile(zipPath, inZipLocation, stream.str().data(), length);
    }

    Bool ZipFileOperations::WriteBufferToZipFile(
        const String& zipPath, const String& inZipLocation, char* data, UInt64 length) {
        if (data == NULL || length == 0) return false;

        void* handle = NULL;
        Int32 err = MZ_OK;

        mz_zip_file file_info = {0};

        file_info.filename = inZipLocation.C_Str();
        file_info.compression_method = s_ZipCompressionMethod;
        file_info.flag = MZ_ZIP_FLAG_UTF8 | MZ_ZIP_FLAG_ENCRYPTED;
        file_info.modified_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        file_info.zip64 = 1;
        file_info.pk_verify = 1;

        mz_zip_writer_create(&handle);
        mz_zip_writer_set_password(handle, s_ZipPassword);
        mz_zip_writer_set_compress_method(handle, s_ZipCompressionMethod);
        mz_zip_writer_set_compress_level(handle, s_ZipCompressionLevel);

        err = mz_zip_writer_open_file(handle, zipPath.C_Str(), 0, 1);

        err = mz_zip_writer_entry_open(handle, &file_info);
        err = mz_zip_writer_entry_write(handle, data, length);
        err = mz_zip_writer_entry_close(handle);

        err = mz_zip_writer_close(handle);

        mz_zip_writer_delete(&handle);
        return err == MZ_OK;
    }
}  // namespace Sentinel