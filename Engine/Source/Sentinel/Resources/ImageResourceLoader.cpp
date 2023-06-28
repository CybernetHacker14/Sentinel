#include "stpch.h"
#include "Sentinel/Resources/ImageResourceLoader.h"

#include "Sentinel/Memory/Malloc.h"

// Created using little-endianness in mind

namespace Sentinel {
    void ImageResourceLoader::SaveToFile(const STL::string& path, ImageResource* inResource) {
        UInt32 heightOffset = sizeof(UInt8) + sizeof(UInt16);
        UInt32 pixelDataOffset = heightOffset + sizeof(UInt16);
        UInt64 pixelDataLength = inResource->Height * inResource->Width * inResource->Channels;

        UInt64 length = pixelDataLength + pixelDataOffset;  // Length of buffer

        UInt8* bytes = (UInt8*)Malloc(length);
        memmove(bytes, &(inResource->Channels), sizeof(UInt8));
        memmove(bytes + sizeof(UInt8), &(inResource->Width), sizeof(UInt16));
        memmove(bytes + heightOffset, &(inResource->Height), sizeof(UInt16));
        memmove(bytes + pixelDataOffset, inResource->Pixels, pixelDataLength);

        Filesystem::WriteToFileAtPath(path, (UInt8*)bytes, length);
        Free(bytes);
    }

    Bool ImageResourceLoader::SaveToArchive(
        const STL::string& zipFilepath, const STL::string& inZipLocation, ImageResource* inResource) {
        return false;
    }

    void ImageResourceLoader::LoadFromFile(const STL::string& path, ImageResource** outResource) {
        UInt32 size = Filesystem::GetFileSize(path);
        UInt8* buffer = Filesystem::ReadFileAtPath(path);

        *outResource = new ImageResource();
        (*outResource)->Channels = buffer[0];
        (*outResource)->Width = buffer[1] + (uint16_t)(buffer[2] << 8);
        (*outResource)->Height = buffer[3] + (uint16_t)(buffer[4] << 8);
        (*outResource)->Pixels = &buffer[5];
    }

    Bool ImageResourceLoader::LoadFromArchive(
        const STL::string& zipFilepath, const STL::string& inZipLocation, ImageResource** outResource) {
        return false;
    }
}  // namespace Sentinel