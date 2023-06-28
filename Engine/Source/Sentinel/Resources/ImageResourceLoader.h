#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Resources/Resource.h"
#include "Sentinel/Filesystem/Filesystem.h"

namespace Sentinel {
    struct ImageResource {
    public:
        inline ~ImageResource() { delete[] Pixels; }

    public:
        UInt8* Pixels;
        UInt16 Width, Height;
        UInt8 Channels;
    };

    class ImageResourceLoader {
    public:
        static void SaveToFile(const STL::string& path, ImageResource* inResource);

        static Bool SaveToArchive(
            const STL::string& zipFilepath, const STL::string& inZipLocation, ImageResource* inResource);

        static void LoadFromFile(const STL::string& path, ImageResource** outResource);

        static Bool LoadFromArchive(
            const STL::string& zipFilepath, const STL::string& inZipLocation, ImageResource** outResource);
    };
}  // namespace Sentinel