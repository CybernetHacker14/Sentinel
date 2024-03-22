#include "stpch.h"
#include "Graphics/D3D11/Buffer/D3D11VertexLayout.h"
#include "Graphics/D3D11/D3D11Renderer.h"
#include "Graphics/D3D11/Device/D3D11Device.h"

// https://gist.github.com/msmshazan/dfd5362004be37ff5e016b6a42be5083

#pragma warning(push)
#pragma warning(disable : 6011)

void Sentinel_D3D11Renderer_Init() {
    Sentinel_D3D11Device_Init();
}

void Sentinel_D3D11Renderer_PreUpdateInit() {
    Sentinel_D3D11VertexLayout_Bind();
}

void Sentinel_D3D11Renderer_OnUpdate() {
}

void Sentinel_D3D11Renderer_OnRender() {
    Sentinel_D3D11Device_Draw();
}

void Sentinel_D3D11Renderer_Deinit() {
    Sentinel_D3D11VertexLayout_Deinit();
    Sentinel_D3D11Device_Deinit();
}

#pragma warning(pop)
