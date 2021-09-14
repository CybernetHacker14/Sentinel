#include "stpch.h"

#include "Platform/DirectX11/Graphics/Modules/DX11Pipeline.h"
#include "Platform/DirectX11/Graphics/Components/Materials/DX11Shader.h"

namespace Sentinel
{
	STL::unordered_map<DXGI_FORMAT, UInt> ShaderDataTypeSizeMap = {
		{DXGI_FORMAT_R32_FLOAT,          4},
		{DXGI_FORMAT_R32_UINT,           4},
		{DXGI_FORMAT_R32_SINT,           4},
		{DXGI_FORMAT_R32G32_FLOAT,       8},
		{DXGI_FORMAT_R32G32_UINT,        8},
		{DXGI_FORMAT_R32G32_SINT,        8},
		{DXGI_FORMAT_R32G32B32_FLOAT,   12},
		{DXGI_FORMAT_R32G32B32_UINT,    12},
		{DXGI_FORMAT_R32G32B32_SINT,    12},
		{DXGI_FORMAT_R32G32B32A32_FLOAT,16},
		{DXGI_FORMAT_R32G32B32A32_UINT, 16},
		{DXGI_FORMAT_R32G32B32A32_SINT, 16}
	};

	DX11Pipeline::DX11Pipeline() {}

	DX11Pipeline::~DX11Pipeline() {
		if (m_InputLayout)
			m_InputLayout->Release();
	}

	void DX11Pipeline::CreateInputLayout(Ref<ShaderBase> shader) {

		DX11Shader* dshader = shader->DerivedDowncast<DX11Shader>();
		ID3DBlob* vertexShaderBinary = shader->DerivedDowncast<DX11Shader>()->GetVertexShaderBinary();

		// Excerpts from https://gist.github.com/mobius/b678970c61a93c81fffef1936734909f

		ID3D11ShaderReflection* vertexShaderReflection = nullptr;
		if (FAILED(D3DReflect(vertexShaderBinary->GetBufferPointer(), vertexShaderBinary->GetBufferSize(),
			IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection)))
		{
			ST_ENGINE_ASSERT(false, "Cannot reflect DirectX11 Vertex Shader!")
		}

		D3D11_SHADER_DESC shaderDescription;
		vertexShaderReflection->GetDesc(&shaderDescription);

		m_Stride = 0;

		STL::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDescriptions;
		for (UInt i = 0; i < shaderDescription.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDescription;
			vertexShaderReflection->GetInputParameterDesc(i, &paramDescription);

			D3D11_INPUT_ELEMENT_DESC elementDescription;
			elementDescription.SemanticName = paramDescription.SemanticName;
			elementDescription.SemanticIndex = paramDescription.SemanticIndex;
			elementDescription.InputSlot = 0;
			elementDescription.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDescription.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDescription.InstanceDataStepRate = 0;

			if (paramDescription.Mask == 1)
			{
				if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDescription.Format = DXGI_FORMAT_R32_FLOAT;
				else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDescription.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDescription.Format = DXGI_FORMAT_R32_SINT;
			}
			else if (paramDescription.Mask <= 3)
			{
				if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDescription.Format = DXGI_FORMAT_R32G32_FLOAT;
				else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDescription.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDescription.Format = DXGI_FORMAT_R32G32_SINT;
			}
			else if (paramDescription.Mask <= 7)
			{
				if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDescription.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDescription.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDescription.Format = DXGI_FORMAT_R32G32B32_SINT;
			}
			else if (paramDescription.Mask <= 15)
			{
				if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDescription.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDescription.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			}

			inputLayoutDescriptions.emplace_back(elementDescription);

			m_Stride += ShaderDataTypeSizeMap.at(elementDescription.Format);
		}

		DX11Common::GetDevice()->CreateInputLayout(
			&inputLayoutDescriptions[0], static_cast<UInt>(inputLayoutDescriptions.size()),
			vertexShaderBinary->GetBufferPointer(), vertexShaderBinary->GetBufferSize(), &m_InputLayout);

		vertexShaderReflection->Release();
	}

	void DX11Pipeline::Bind() {
		DX11Common::GetContext()->IASetInputLayout(m_InputLayout);
	}

	void DX11Pipeline::Unbind() {
		DX11Common::GetContext()->IASetInputLayout(nullptr);
	}
}
