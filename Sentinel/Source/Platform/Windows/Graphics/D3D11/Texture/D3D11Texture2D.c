#include "stpch.h"
#include "_EXPORT/Graphics/Texture/Texture2DAPI_EXPORT.h"
#include "_EXPORT/Graphics/Texture/Texture2DData_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Common/Core/Macros.h"
#include "Graphics/D3D11/Device/D3D11Device.h"
#include "Graphics/D3D11/Texture/D3D11Texture2D.h"

#ifdef ST_DEBUG
    #include "Logging/Log.h"
#endif  // ST_DEBUG

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_GIF
#include <stb_image.h>

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>

#pragma warning(push)
#pragma warning(disable : 6011)

// This list is populated according to the index values of the TextureWrapMode enum
static D3D11_TEXTURE_ADDRESS_MODE addressModeList[] = {
    D3D11_TEXTURE_ADDRESS_WRAP,
    D3D11_TEXTURE_ADDRESS_CLAMP,
    D3D11_TEXTURE_ADDRESS_MIRROR,
    D3D11_TEXTURE_ADDRESS_MIRROR_ONCE};

static FixedSlabAllocator tex2DNativeAllocator;

static void Sentinel_D3D11Texture2D_BindToVertexShader(Texture2DData* texture) {
    ID3D11DeviceContext_VSSetShaderResources(
        Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &((Tex2DD3D11Native*)texture->native)->nativeSRV);
    ID3D11DeviceContext_VSSetSamplers(
        Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &((Tex2DD3D11Native*)texture->native)->sampler);
}

static void Sentinel_D3D11Texture2D_BindToPixelShader(Texture2DData* texture) {
    ID3D11DeviceContext_PSSetShaderResources(
        Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &((Tex2DD3D11Native*)texture->native)->nativeSRV);
    ID3D11DeviceContext_PSSetSamplers(
        Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &((Tex2DD3D11Native*)texture->native)->sampler);
}

static void Sentinel_D3D11Texture2D_BindToComputeShader(Texture2DData* texture) {
    ID3D11DeviceContext_CSSetShaderResources(
        Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &((Tex2DD3D11Native*)texture->native)->nativeSRV);
    ID3D11DeviceContext_CSSetSamplers(
        Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &((Tex2DD3D11Native*)texture->native)->sampler);
}

static void Sentinel_D3D11Texture2D_UnbindFromVertexShader(Texture2DData* texture) {
    ID3D11ShaderResourceView* nullSRV = NULL;
    ID3D11SamplerState* nullSampler = NULL;

    ID3D11DeviceContext_VSSetShaderResources(Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &nullSRV);
    ID3D11DeviceContext_VSSetSamplers(Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &nullSampler);
}

static void Sentinel_D3D11Texture2D_UnbindFromPixelShader(Texture2DData* texture) {
    ID3D11ShaderResourceView* nullSRV = NULL;
    ID3D11SamplerState* nullSampler = NULL;

    ID3D11DeviceContext_PSSetShaderResources(Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &nullSRV);
    ID3D11DeviceContext_PSSetSamplers(Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &nullSampler);
}

static void Sentinel_D3D11Texture2D_UnbindFromComputeShader(Texture2DData* texture) {
    ID3D11ShaderResourceView* nullSRV = NULL;
    ID3D11SamplerState* nullSampler = NULL;

    ID3D11DeviceContext_CSSetShaderResources(Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &nullSRV);
    ID3D11DeviceContext_CSSetSamplers(Sentinel_D3D11Device_GetContext(), texture->bindSlot, 1, &nullSampler);
}

static void (*textureBindPFn[])(Texture2DData*) = {
    Sentinel_D3D11Texture2D_BindToVertexShader,
    Sentinel_D3D11Texture2D_BindToPixelShader,
    Sentinel_D3D11Texture2D_BindToComputeShader};

static void (*textureUnbindPFn[])(Texture2DData*) = {
    Sentinel_D3D11Texture2D_UnbindFromVertexShader,
    Sentinel_D3D11Texture2D_UnbindFromPixelShader,
    Sentinel_D3D11Texture2D_UnbindFromComputeShader};

void Sentinel_D3D11Texture2D_Init() {
    Sentinel_FixedSlabAllocator_Allocate(&tex2DNativeAllocator, sizeof(Tex2DD3D11Native), 32);
}

void Sentinel_D3D11Texture2D_Deinit() {
    Sentinel_FixedSlabAllocator_Deallocate(&tex2DNativeAllocator);
}

void Sentinel_D3D11Texture2D_Create(Texture2DData* texture, Texture2DImportSettings* settings, CChar* filepath) {
    UShort index;
    texture->native = Sentinel_FixedSlabAllocator_New(&tex2DNativeAllocator, &index);

    D3D11_TEXTURE2D_DESC texDesc;
    SecureZeroMemory(&texDesc, sizeof(texDesc));
    texDesc.ArraySize = 1;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = settings->ReadWriteEnabled ? D3D11_USAGE_STAGING : D3D11_USAGE_DEFAULT;
    texDesc.CPUAccessFlags = settings->ReadWriteEnabled ? D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE : 0;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.MiscFlags = 0;  // D3D11_RESOURCE_MISC_GENERATE_MIPS

    texture->pixels = NULL;
    Int width = 0, height = 0, channels = 0;
    texture->properties |= stbi_is_hdr(filepath) ? ST_BIT(1) : texture->properties;

    stbi_set_flip_vertically_on_load(0);

    texture->pixels = (void*)(stbi_load(filepath, &width, &height, &channels, 4));

#ifdef ST_DEBUG
    if (texture->pixels == NULL) {
        ST_TERMINAL_ERROR("Failed to load image at path : %s", filepath);
        return;
    }
#endif  // ST_DEBUG

    texDesc.Format = Sentinel_Texture2DAPI_IsHDR(texture)
                         ? DXGI_FORMAT_R32G32B32A32_FLOAT
                         : (settings->sRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM);

    texture->width = (UShort)width;
    texture->height = (UShort)height;

    texDesc.Width = (UInt)width;
    texDesc.Height = (UInt)height;
    texDesc.MipLevels = (UInt)Sentinel_Texture2DAPI_IsHDR(texture);

    ID3D11Device_CreateTexture2D(
        Sentinel_D3D11Device_GetDevice(), &texDesc, NULL, &(((Tex2DD3D11Native*)texture->native)->nativeTex));

    UInt rowPitch = width * 4 * (Sentinel_Texture2DAPI_IsHDR(texture) ? sizeof(float) : sizeof(unsigned char));
    ID3D11DeviceContext_UpdateSubresource(
        Sentinel_D3D11Device_GetContext(),
        (ID3D11Resource*)((Tex2DD3D11Native*)texture->native)->nativeTex,
        0,
        NULL,
        texture->pixels,
        rowPitch,
        0);

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    SecureZeroMemory(&viewDesc, sizeof(viewDesc));
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MostDetailedMip = 0;
    viewDesc.Texture2D.MipLevels = -1;

    ID3D11Device_CreateShaderResourceView(
        Sentinel_D3D11Device_GetDevice(),
        (ID3D11Resource*)((Tex2DD3D11Native*)texture->native)->nativeTex,
        &viewDesc,
        &(((Tex2DD3D11Native*)texture->native)->nativeSRV));

    D3D11_SAMPLER_DESC samplerDesc;
    SecureZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = addressModeList[(Int)texture->wrapMode];
    samplerDesc.AddressV = addressModeList[(Int)texture->wrapMode];
    samplerDesc.AddressW = addressModeList[(Int)texture->wrapMode];
    samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ID3D11Device_CreateSamplerState(
        Sentinel_D3D11Device_GetDevice(), &samplerDesc, &(((Tex2DD3D11Native*)texture->native)->sampler));
}

void Sentinel_D3D11Texture2D_Destroy(Texture2DData* texture) {
    ID3D11Texture2D* tex = ((Tex2DD3D11Native*)texture->native)->nativeTex;
    ID3D11ShaderResourceView* srv = ((Tex2DD3D11Native*)texture->native)->nativeSRV;
    ID3D11SamplerState* sampler = ((Tex2DD3D11Native*)texture->native)->sampler;

    if (tex) ID3D11Texture2D_Release(tex);
    if (srv) ID3D11ShaderResourceView_Release(srv);
    if (sampler) ID3D11SamplerState_Release(sampler);

    free(texture->pixels);

    UShort index;
    Sentinel_FixedSlabAllocator_Delete(&tex2DNativeAllocator, texture->native, &index);
}

void Sentinel_D3D11Texture2D_Bind(Texture2DData* texture, ShaderType bindType, UShort bindSlot) {
    texture->bindType = bindType;
    texture->bindSlot = bindSlot;
    textureBindPFn[(UInt)texture->bindType](texture);
}

void Sentinel_D3D11Texture2D_Unbind(Texture2DData* texture) {
    textureUnbindPFn[(UInt)texture->bindType](texture);
}

#pragma warning(pop)
