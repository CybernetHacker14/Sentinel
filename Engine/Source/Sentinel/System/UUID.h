#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class UUID {
    public:
        UUID();
        UUID(UInt64 uuid);
        UUID(const UUID& other);

        operator UInt64() { return m_UUID; }
        operator const UInt64() const { return m_UUID; }

        inline STL::string ToString() const { return STL::to_string(m_UUID); }

        inline UUID& FromString(const char* data) {
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

namespace eastl {
    template<typename T>
    struct hash;

    template<>
    struct hash<Sentinel::UUID> {
        Sentinel::Size_t operator()(const Sentinel::UUID& uuid) const { return (Sentinel::UInt64)uuid; }
    };
}  // namespace eastl
