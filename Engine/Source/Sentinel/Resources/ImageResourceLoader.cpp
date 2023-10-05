#include "stpch.h"
#include "Sentinel/Resources/ImageResourceLoader.h"
#include "Sentinel/Filesystem/Filesystem.h"

#include "Sentinel/Common/Core/Malloc.h"
#include "Sentinel/Common/Strings/MemFunctions.h"

// Created using little-endianness in mind

namespace Sentinel {
    void ImageResourceLoader::SaveToFile(CChar* path, ImageResource* inResource) {
        UInt32 heightOffset = sizeof(UInt8) + sizeof(UInt16);
        UInt32 pixelDataOffset = heightOffset + sizeof(UInt16);
        UInt64 pixelDataLength = inResource->Height * inResource->Width * inResource->Channels;

        UInt64 length = pixelDataLength + pixelDataOffset;  // Length of buffer

        UInt8* bytes = (UInt8*)Malloc(length);
        MemFunctions::Memcpy(bytes, &(inResource->Channels), sizeof(UInt8));
        MemFunctions::Memcpy(bytes + sizeof(UInt8), &(inResource->Width), sizeof(UInt16));
        MemFunctions::Memcpy(bytes + heightOffset, &(inResource->Height), sizeof(UInt16));
        MemFunctions::Memcpy(bytes + pixelDataOffset, inResource->Pixels, pixelDataLength);

        Filesystem::WriteToFileAtPath(path, (UInt8*)bytes, length);
        Free(bytes);
    }

    Bool ImageResourceLoader::SaveToArchive(CChar* zipFilepath, CChar* inZipLocation, ImageResource* inResource) {
        return false;
    }

    void ImageResourceLoader::LoadFromFile(CChar* path, ImageResource** outResource) {
        Int64 size = Filesystem::GetFileSize(path);
        UInt8* buffer = (UInt8*)Malloc(size);
        Filesystem::ReadFileAtPath(path, buffer, size);

        *outResource = new ImageResource();
        (*outResource)->Channels = buffer[0];
        (*outResource)->Width = buffer[1] + (uint16_t)(buffer[2] << 8);
        (*outResource)->Height = buffer[3] + (uint16_t)(buffer[4] << 8);
        (*outResource)->Pixels = &buffer[5];
    }

    Bool ImageResourceLoader::LoadFromArchive(CChar* zipFilepath, CChar* inZipLocation, ImageResource** outResource) {
        return false;
    }
}  // namespace Sentinel