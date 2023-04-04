#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class ZipFileOperations {
    public:
        static Bool DoesFileExistInZip(const STL::string& zipPath, const STL::string& inZipLocation);

        static Bool ReadFromZipFile(const STL::string& zipPath, const STL::string& inZipLocation, void** ptrToBuffer);

        static Bool WriteFileToZipFile(
            const STL::string& zipPath, const STL::string& inZipLocation, const STL::string& filepath);

        static Bool WriteBufferToZipFile(
            const STL::string& zipPath, const STL::string& inZipLocation, std::stringstream stream);

        static Bool WriteBufferToZipFile(
            const STL::string& zipPath, const STL::string& inZipLocation, char* data, UInt32 length);
    };
}  // namespace Sentinel
