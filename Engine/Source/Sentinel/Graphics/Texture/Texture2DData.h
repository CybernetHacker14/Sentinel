#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Material/ShaderData.h"
#include "Sentinel/Graphics/Device/ContextData.h"

namespace Sentinel {
    enum class TextureWrapMode : UInt8 { REPEAT = 0, CLAMP = 1, MIRROR = 2 };

    struct Texture2DDataImportSettings {
        STL::string TextureFilepath;

        TextureWrapMode WrapMode = TextureWrapMode::CLAMP;

        Bool ReadWriteEnabled = false;
        Bool GenerateMipMaps = false;
        Bool sRGB = false;
    };

    struct Texture2DData {
    public:
        ContextData* Context;

    protected:
        Texture2DDataImportSettings m_Settings;
        void* m_TexturePixels = nullptr;
        UInt32 m_Width = 0, m_Height = 0;
        UInt32 m_BindSlot;
        ShaderType m_BindType = ShaderType::NONE;
        Bool m_Loaded = false, m_HDR = false;

    private:
        friend class Texture2DAPI;
    };
}  // namespace Sentinel