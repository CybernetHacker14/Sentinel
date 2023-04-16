#pragma once

#include "Sentinel/Common/Common.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Sentinel {
    enum class ResourceType : UInt8 {
        RESOURCE_TYPE_TEXT = 1,
        RESOURCE_TYPE_BINARY = 2,
        RESOURCE_TYPE_IMAGE = 3,
        RESOURCE_TYPE_CUSTOM = 4
    };

    struct Resource {
        UInt32 LoaderID;
        const char* Name;
        char* Fullpath;
        UInt64 DataSize;
        void* Data;
    };

    struct ImageResource {
        UInt8 Channels;
        UInt16 Width;
        UInt16 Height;
        UInt8* Pixels;

        template<class Archive>
        void save(Archive& archive) const {
            archive(cereal::make_nvp("Channels", Channels);
            archive(cereal::make_nvp("Width", Width);
            archive(cereal::make_nvp("Height", Height);
            archive(cereal::binary_data(Pixels, sizeof(UInt8) * Width * Height * Channels));
        }

        template<class Archive>
        void load(Archive& archive) {
            archive(cereal::make_nvp("Channels", Channels);
            archive(cereal::make_nvp("Width", Width);
            archive(cereal::make_nvp("Height", Height);
            archive(cereal::binary_data(Pixels, sizeof(UInt8) * Width * Height * Channels));
        }
    };

}  // namespace Sentinel
