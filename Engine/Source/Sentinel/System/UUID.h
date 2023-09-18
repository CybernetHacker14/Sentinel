#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    class UUID {
    public:
        UUID();
        UUID(UInt64 uuid);
        UUID(const UUID& other);

        operator UInt64() { return m_UUID; }
        operator const UInt64() const { return m_UUID; }

        // There's a faster way implemented in fmt library, but it's tedious to implement
        // Requires checking for builtin clz and ctz intrinsics
        inline CChar* ToString(Char* buffer) const {
            sprintf_s(buffer, 21, "%llu", m_UUID);
            return buffer;
        }

        inline UUID& FromString(CChar* data) {
            m_UUID = strtoull(data, nullptr, 10);
            return *this;
        };

    private:
        UInt64 m_UUID;
    };
}  // namespace Sentinel

namespace std {
    template<typename T>
    struct hash;

    template<>
    struct hash<Sentinel::UUID> {
        Sentinel::Size_t operator()(const Sentinel::UUID& uuid) const { return (Sentinel::UInt64)uuid; }
    };
}  // namespace std
