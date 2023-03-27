#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class ZipFileOperations {
    public:
        static Bool DoesFileExistInZip(const STL::string& zipPath, const STL::string& inZipLocation);

        static Bool ReadFromZipFile(const STL::string& zipPath, const STL::string& inZipLocation, void** ptrToBuffer);

        static Bool WriteToZipFile(
            const STL::string& zipPath, const STL::string& inZipLocation, std::stringstream stream);

        static Bool WriteToZipFile(
            const STL::string& zipPath, const STL::string& inZipLocation, const char* data, UInt32 length);

        static Bool AddToZipFile(
            const STL::string& zipPath, const STL::string& inZipLocation, const STL::string& diskFilePath);
    };
}  // namespace Sentinel
