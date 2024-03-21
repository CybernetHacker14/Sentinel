#include "stpch.h"
#include "_EXPORT/Graphics/Buffer/ConstantbufferAPI_EXPORT.h"
#include "_EXPORT/Graphics/Buffer/ConstantbufferData_EXPORT.h"
#include "_EXPORT/Graphics/Material/ShaderData_EXPORT.h"
#include "Graphics/Buffer/Constantbuffer.h"
#include "Graphics/D3D11/Buffer/D3D11Constantbuffer.h"
#include "Graphics/D3D11/Device/D3D11Device.h"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>

#pragma warning(push)
#pragma warning(disable : 6011)

static void VSBind(ConstantbufferData* buffer) {
    ID3D11Buffer* cBuffer = (ID3D11Buffer*)(buffer->native);
    ID3D11DeviceContext_VSSetConstantBuffers(Sentinel_D3D11Device_GetContext(), buffer->bindSlot, 1, &cBuffer);
}

static void PSBind(ConstantbufferData* buffer) {
    ID3D11Buffer* cBuffer = (ID3D11Buffer*)(buffer->native);
    ID3D11DeviceContext_PSSetConstantBuffers(Sentinel_D3D11Device_GetContext(), buffer->bindSlot, 1, &cBuffer);
}

static void CSBind(ConstantbufferData* buffer) {
    ID3D11Buffer* cBuffer = (ID3D11Buffer*)(buffer->native);
    ID3D11DeviceContext_CSSetConstantBuffers(Sentinel_D3D11Device_GetContext(), buffer->bindSlot, 1, &cBuffer);
}

static void SetStaticData(ConstantbufferData* buffer, void* data) {
    ID3D11Buffer* cBuffer = (ID3D11Buffer*)(buffer->native);
    ID3D11DeviceContext_UpdateSubresource(
        Sentinel_D3D11Device_GetContext(), (ID3D11Resource*)cBuffer, 0, 0, data, 0, 0);
}

static void SetDynamicData(ConstantbufferData* buffer, void* data) {
    ID3D11Buffer* cBuffer = (ID3D11Buffer*)(buffer->native);
    D3D11_MAPPED_SUBRESOURCE subresource;
    SecureZeroMemory(&subresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
    ID3D11DeviceContext_Map(
        Sentinel_D3D11Device_GetContext(), (ID3D11Resource*)cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
    Sentinel_Memcpy(subresource.pData, data, buffer->size);
    ID3D11DeviceContext_Unmap(Sentinel_D3D11Device_GetContext(), (ID3D11Resource*)cBuffer, 0);
}

static void (*cBufferBindPFn[])(ConstantbufferData*) = {VSBind, PSBind, CSBind};
static void (*cBufferSetDataPFn[])(ConstantbufferData*, void*) = {SetStaticData, SetDynamicData};

void Sentinel_D3D11Constantbuffer_Create(ConstantbufferData* buffer) {
    D3D11_BUFFER_DESC desc;
    SecureZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
    desc.Usage = buffer->usageType == DEFAULT ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = (UInt)(buffer->size);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = buffer->usageType == DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    ID3D11Buffer* cBuffer = NULL;
    ID3D11Device_CreateBuffer(Sentinel_D3D11Device_GetDevice(), &desc, NULL, &cBuffer);
    buffer->native = cBuffer;
}

void Sentinel_D3D11Constantbuffer_Destroy(ConstantbufferData* buffer) {
    ID3D11Buffer* cBuffer = (ID3D11Buffer*)(buffer->native);
    ID3D11Buffer_Release(cBuffer);
}

void Sentinel_D3D11Constantbuffer_Bind(ConstantbufferData* buffer, ShaderType type) {
    cBufferBindPFn[type](buffer);
}

void Sentinel_D3D11Constantbuffer_SetData(ConstantbufferData* buffer, void* data) {
    cBufferSetDataPFn[buffer->usageType](buffer, data);
}

#pragma warning(pop)
