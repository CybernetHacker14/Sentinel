#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    class ZipFileOperations {
    public:
        static Bool DoesFileExistInZip(CChar* zipPath, CChar* inZipLocation);

        static Bool ReadFromZipFile(CChar* zipPath, CChar* inZipLocation, void** ptrToBuffer, UInt32& dataLength);

        static Bool WriteTextFileToZipFile(CChar* zipPath, CChar* inZipLocation, CChar* filepath);

        static Bool WriteBufferToZipFile(CChar* zipPath, CChar* inZipLocation, std::stringstream stream);

        static Bool WriteBufferToZipFile(CChar* zipPath, CChar* inZipLocation, CChar* data, UInt64 length);
    };
}  // namespace Sentinel
