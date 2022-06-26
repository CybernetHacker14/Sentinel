#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Buffer/ConstantbufferAPI.h"

namespace Sentinel {
    struct DX11ConstantbufferData;

    class DX11ConstantbufferAPI final: public ConstantbufferAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_VSBindFunction = DX11ConstantbufferAPI::VSBind;
                    m_PSBindFunction = DX11ConstantbufferAPI::PSBind;
                    m_CSBindFunction = DX11ConstantbufferAPI::CSBind;

                    m_SetStaticDataFunction = DX11ConstantbufferAPI::SetStaticData;
                    m_SetDynamicDataFunction = DX11ConstantbufferAPI::SetDynamicData;

                    m_CleanFunction = DX11ConstantbufferAPI::Clean;
                }
            }
        } _initializer;

    private:
        static void VSBind(ConstantbufferData* dataObject);
        static void PSBind(ConstantbufferData* dataObject);
        static void CSBind(ConstantbufferData* dataObject);

        static void SetStaticData(ConstantbufferData* dataObject, void* data);
        static void SetDynamicData(ConstantbufferData* dataObject, void* data);

        static void Clean(ConstantbufferData* dataObject);

        static void CreateNative(
            DX11ConstantbufferData* dataObject, UInt32 size, UInt32 bindSlot, CBufferUsageType usageType);

    private:
        friend class ConstantbufferAPI;
    };
}  // namespace Sentinel