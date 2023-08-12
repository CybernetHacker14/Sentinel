#pragma once

#include "Sentinel/Common/Strings/String.h"

namespace Sentinel {
    class ZipFileOperations {
    public:
        static Bool DoesFileExistInZip(const String& zipPath, const String& inZipLocation);

        static Bool ReadFromZipFile(
            const String& zipPath, const String& inZipLocation, void** ptrToBuffer, UInt32& dataLength);

        static Bool WriteTextFileToZipFile(
            const String& zipPath, const String& inZipLocation, const String& filepath);

        static Bool WriteBufferToZipFile(
            const String& zipPath, const String& inZipLocation, std::stringstream stream);

        static Bool WriteBufferToZipFile(
            const String& zipPath, const String& inZipLocation, char* data, UInt64 length);
    };
}  // namespace Sentinel
