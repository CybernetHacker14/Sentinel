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

}  // namespace Sentinel
