#include "stpch.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Components/Materials/DX11Shader.h"

namespace Sentinel
{
	DX11Shader::DX11Shader(const STL::string& filepath) {}

	DX11Shader::~DX11Shader() {}

	void DX11Shader::Bind() const {}

	void DX11Shader::Reload() {
		m_VertexShader = nullptr;
		m_PixelShader = nullptr;
		m_ComputeShader = nullptr;
	}

	const STL::string& DX11Shader::GetShaderSource(const ShaderType& type) const {
		// TODO: insert return statement here
		return "";
	}
}
