#include "stpch.h"
#include "Platform/DirectX11/Renderer/Core/DirectX11Internal.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <fstream>

namespace Sentinel
{
	DirectX11Internal* DirectX11Internal::s_Internal = new DirectX11Internal();

	DirectX11Internal::~DirectX11Internal() {
		pPS->Release();
		pVS->Release();
		m_Swapchain->Release();
		m_BackBuffer->Release();
		m_Device->Release();
		m_Context->Release();

		delete pPS;
		delete pVS;

		delete m_Swapchain;
		delete m_BackBuffer;
		delete m_Device;
		delete m_Context;

		delete m_AdapterDescription;
		delete s_Internal;
	}

	void DirectX11Internal::Init(GLFWwindow* windowHandle) {
		DXGI_SWAP_CHAIN_DESC description;
		ZeroMemory(&description, sizeof(DXGI_SWAP_CHAIN_DESC));

		description.BufferDesc.Width = 0;
		description.BufferDesc.Height = 0;
		description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		description.BufferDesc.RefreshRate.Numerator = 0;
		description.BufferDesc.RefreshRate.Denominator = 0;
		description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		description.SampleDesc.Count = 4;
		description.SampleDesc.Quality = 0;

		description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		description.BufferCount = 1;

		description.OutputWindow = glfwGetWin32Window(windowHandle);

		description.Windowed = true;

		description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		description.Flags = 0;

		// Create the device and the swapchain
		auto result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&description,
			&m_Swapchain,
			&m_Device,
			nullptr,
			&m_Context
		);

		m_Device->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&m_DXGIDevice);
		m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (LPVOID*)&m_Adapter);

		m_AdapterDescription = new DXGI_ADAPTER_DESC();
		m_Adapter->GetDesc(m_AdapterDescription);

		ID3D11Texture2D* backBuffer = nullptr;

		// Get the address of the back buffer
		m_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

		// use the back buffer address to create the render target
		m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_BackBuffer);
		backBuffer->Release();

		m_Context->OMSetRenderTargets(1, &m_BackBuffer, nullptr);

		ID3D10Blob* VS, * PS;
		ID3D10Blob* errorMessages;

		D3DCompileFromFile(L"shaders.shader", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, &VS, &errorMessages);
		D3DCompileFromFile(L"shaders.shader", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, &PS, &errorMessages);

		m_Device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &pVS);
		m_Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pPS);

		m_Context->VSSetShader(pVS, 0, 0);
		m_Context->PSSetShader(pPS, 0, 0);

		Vertex vertices[] =
		{
			{ 0.0f, 0.0f, 0.0f },
			{ 0.5f, -0.5, 0.0f},
			{-0.5f, -0.5f, 0.0f}
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(Vertex) * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		pVBuffer = nullptr;
		m_Device->CreateBuffer(&bd, nullptr, &pVBuffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		m_Context->Map(pVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, vertices, sizeof(vertices));
		m_Context->Unmap(pVBuffer, 0);

		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		ID3D11InputLayout* pLayout;

		m_Device->CreateInputLayout(ied, 1, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
		m_Context->IASetInputLayout(pLayout);
	}

	void DirectX11Internal::Draw() {
		uint32_t stride = sizeof(float) * 3;
		uint32_t offset = 0;
		m_Context->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
		m_Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_Context->Draw(3, 0);
	}

	DirectX11Internal* DirectX11Internal::GetInternalHandle() {
		return s_Internal;
	}
}
