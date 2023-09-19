#pragma once

#include "Sentinel/Resources/Resource.h"

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
        static void SaveToFile(CChar* path, ImageResource* inResource);

        static Bool SaveToArchive(CChar* zipFilepath, CChar* inZipLocation, ImageResource* inResource);

        static void LoadFromFile(CChar* path, ImageResource** outResource);

        static Bool LoadFromArchive(CChar* zipFilepath, CChar* inZipLocation, ImageResource** outResource);
    };
}  // namespace Sentinel