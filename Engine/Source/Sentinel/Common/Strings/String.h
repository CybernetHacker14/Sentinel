#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

// Didn't make it compiler-based, because we are using a union and
// with 15, both the heap and sso size becomes same
#define SHORT_STRING_SIZE 15

namespace Sentinel {
    struct String {
    public:
        static constexpr UInt32 npos = static_cast<UInt32>(-1);

    public:
        String();
        String(CChar* data);
        String(const String& other);

        String(String&& other) noexcept;

        ~String();

        String& operator=(const String& other);
        String& operator=(String&& other) noexcept;

        inline Char& operator[](UInt32 index);
        inline CChar& operator[](UInt32 index) const;

        inline Char* Data();
        inline CChar* C_Str() const;

        inline UInt32 Length() const;

        UInt64 Hash() const;

        // This means you are explicitly overriding SSO
        inline void SetHeapCapacity(UInt32 capacity);

        inline Bool Empty() const;

        String& Append(CChar letter);
        String& Append(const String& other);

        inline friend String& operator+(String string, CChar letter) { return string.Append(letter); }
        inline friend String& operator+(String string, const String& other) { return string.Append(other); }
        inline friend String& operator+(String string, CChar* other) { return string.Append(other); }

        inline String& String::operator+=(CChar letter) { return Append(letter); }
        inline String& String::operator+=(const String& other) { return Append(other); }

    private:
        String& Set(CChar* data, UInt32 size, UInt32 start = 0);
        UInt32 GetSize() const;

    private:
        struct SSOLayout {
            Char Data[SHORT_STRING_SIZE] = {0};
            UInt8 Size = 0;
        };

        struct HeapLayout {
            Char* Data = nullptr;
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

namespace std {
    template<typename T>
    struct hash;

    template<>
    struct hash<Sentinel::String> {
        Sentinel::Size_t operator()(const Sentinel::String& string) const { return string.Hash(); }
    };
}  // namespace std
