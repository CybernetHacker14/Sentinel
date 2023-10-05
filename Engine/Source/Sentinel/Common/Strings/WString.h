#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

// Didn't make it compiler-based, because we are using a union and
// with 7, both the heap and sso size becomes same
#define SHORT_STRING_SIZE 15

namespace Sentinel {
    struct WString {
    public:
        static constexpr UInt32 npos = static_cast<UInt32>(-1);

    public:
        WString();
        WString(CWChar* data);
        WString(const WString& other);

        WString(WString&& other) noexcept;

        ~WString();

        WString& operator=(const WString& other);
        WString& operator=(WString&& other) noexcept;

        inline WChar& operator[](UInt32 index);
        inline CWChar& operator[](UInt32 index) const;

        inline WChar* Data();
        inline CWChar* C_Str() const;

        inline UInt32 Length() const;

        UInt64 Hash() const;

        // This means you are explicitly overriding SSO
        inline void SetHeapCapacity(UInt32 capacity);

        inline Bool Empty() const;

    private:
        WString& Set(CWChar* data, UInt32 size, UInt32 start = 0);
        UInt32 GetSize() const;

    private:
        struct SSOLayout {
            WChar Data[SHORT_STRING_SIZE] = {0};
            UInt16 Size = 0;
        };

        struct HeapLayout {
            WChar* Data = nullptr;
            UInt32 Capacity = 0, Size = 0;
        };

        struct Layout {
            union Representation {
                constexpr Representation() : sso {} {};

                HeapLayout heap;
                SSOLayout sso;
            };

            Layout() { Data.sso.Size = 0; }

            Representation Data;
        };

        Layout m_Data;
        UInt32 m_RemainingSize = 0;
        Bool m_SSO = true;
        Bool m_Empty = true;
        // We can store a refcount of how many string views are
        // referencing this string
        UInt16 m_StringViewRefCount = 0;
    };
}  // namespace Sentinel