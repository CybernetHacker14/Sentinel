#pragma once

#include "Sentinel/Common/Strings/StringView.h"
#include "Sentinel/Common/Containers/Vector.h"

namespace Sentinel {
    // Filesystem I/O functions.
    // WARNING: Avoid too much usage, and mostly aimed for editor functionality

    struct Path {
    public:
        Path(CChar* path);

        Path(const Path& other) = default;
        Path& operator=(const Path& other) = default;

        Path(Path&& other) = default;
        Path& operator=(Path&& other) = default;

        Bool operator==(const Path& rhs) const noexcept;

        inline CChar* GetAbsolutePath() const { return m_AbsolutePath; }

        inline Bool Exists() const { return m_Properties & (ST_BIT(0)); }
        inline Bool IsFile() const { return m_Properties & (ST_BIT(1)); }
        inline Bool IsFolder() const { return m_Properties & (ST_BIT(2)); }
        inline Bool IsReadOnly() const { return m_Properties & (ST_BIT(3)); }
        inline Bool IsHidden() const { return m_Properties & (ST_BIT(4)); }
        inline Bool IsHTMLFile() const { return m_Properties & (ST_BIT(6)); }
        inline Bool IsFileURL() const { return m_Properties & (ST_BIT(7)); }
        inline Bool HasExtension() const { return m_Properties & (ST_BIT(8)); }
        inline Bool DoesFileExist() const { return m_Properties & (ST_BIT(9)); }
        inline Bool DoesFolderExist() const { return m_Properties & (ST_BIT(10)); }
        inline Bool IsFileEmpty() const { return m_Properties & (ST_BIT(11)); }
        inline Bool IsFolderEmpty() const { return m_Properties & (ST_BIT(12)); }

        CChar* GetFilenameWithExtension() const;
        CChar* GetFilenameWithoutExtension() const;
        CChar* GetExtension() const;

    private:
        Path() = default;

    private:
        Char* m_AbsolutePath;

        // DO NOT DELETE
        // 0 - exists, 1 - file, 2 - folder, 3 - readonly, 4 - hidden, 5 - url, 6 - html
        // 7 - file url, 8 - extension, 9 - file exists, 10 - folder exists, 11 - file empty
        // 12 - folder empty

        UInt64 m_Properties = 0;
    };

    class Filesystem {
    public:
        static void CreateFolder(const Path& folderpath);
        static Bool HasSubfolders(const Path& folderpath);
        static Vector<Path> GetImmediateSubfolders(const Path& folderpath);
        static Vector<Path> GetImmediatePaths(const Path& folderpath);
        static const Int64 GetFileSize(const Path& filepath);
        static Bool ReadFileAtPath(const Path& filepath, void* buffer, Int64& outSize);
        static Bool ReadTextFileAtPath(const Path& filepath, Char* buffer, Int64& outSize);
        static Bool WriteToFileAtPath(const Path& filepath, UInt8* buffer, UInt64 length);
        static Bool WriteToTextFileAtPath(const Path& filepath, CChar* text);
        static Bool OpenAtPath(const Path& path);
        static Bool DeleteAtPath(const Path& path);
        static Bool MoveToPath(const Path& currentpath, const Path& newpath);
        static Bool CopyToPath(const Path& currentpath, const Path& newpath);

        static Bool GetParentPath(const Path& currentPath, Char* buffer);
    };
}  // namespace Sentinel

namespace std {
    template<typename T>
    struct hash;

    template<>
    struct hash<Sentinel::Path> {
        Sentinel::Size_t operator()(const Sentinel::Path& path) const {
            Sentinel::Size_t h = 5381;
            Sentinel::CChar* s = path.GetAbsolutePath();
            Sentinel::UInt32 c;
            while ((c = *s++)) h = ((h << 5) + h) + c;
            return h;
        }
    };
}  // namespace std
