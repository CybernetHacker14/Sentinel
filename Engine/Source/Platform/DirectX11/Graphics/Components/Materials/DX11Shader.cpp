#include "stpch.h"

#include "Sentinel/Filesystem/Filesystem.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Components/Materials/DX11Shader.h"

namespace Sentinel
{
	namespace Utils
	{
		static ShaderType ShaderTypeFromString(const STL::string& type) {
			if (type == "vertex")
				return ShaderType::VERTEX;
			if (type == "pixel")
				return ShaderType::PIXEL;
			if (type == "compute")
				return ShaderType::COMPUTE;

			ST_ENGINE_ASSERT(false, "Unknown shader type!");
			return ShaderType::NONE;
		}

		static const char* StringFromShaderType(const ShaderType type) {
			if (type == ShaderType::VERTEX)
				return "vertex";
			if (type == ShaderType::PIXEL)
				return "pixel";
			if (type == ShaderType::COMPUTE)
				return "compute";

			ST_ENGINE_ASSERT(false, "Unknown shader type!");
			return "";
		}

		static const char* ProfileFromShaderType(const ShaderType type) {
			if (type == ShaderType::VERTEX)
				return "vs_5_0";
			if (type == ShaderType::PIXEL)
				return "ps_5_0";
			if (type == ShaderType::COMPUTE)
				return "cs_5_0";

			ST_ENGINE_ASSERT(false, "Unknown shader type!");
			return "";
		}

		static const char* EntryPointFromShaderType(const ShaderType type) {
			if (type == ShaderType::VERTEX)
				return "VShader";
			if (type == ShaderType::PIXEL)
				return "PShader";
			if (type == ShaderType::COMPUTE)
				return "CShader";

			ST_ENGINE_ASSERT(false, "Unknown shader type!");
			return "";
		}
	}

	DX11Shader::DX11Shader(const STL::string& filepath, const STL::string& name)
		:m_FilePath(filepath), m_ShaderName(name) {
		Load();
	}

	DX11Shader::~DX11Shader() {
		for (auto& tuple : m_ShaderBinaryMap)
		{
			tuple.second->Release();
		}

		if (m_VertexShader)
			m_VertexShader->Release();

		if (m_PixelShader)
			m_PixelShader->Release();

		if (m_ComputeShader)
			m_ComputeShader->Release();

		m_ShaderBinaryMap.clear();
		m_ShaderSourceMap.clear();

		m_ShaderName.clear();
		m_FilePath.clear();
	}

	void DX11Shader::Bind() const {
		if (m_VertexShader)
			DX11Common::GetContext()->VSSetShader(m_VertexShader, nullptr, 0);

		if (m_PixelShader)
			DX11Common::GetContext()->PSSetShader(m_PixelShader, nullptr, 0);

		if (m_ComputeShader)
			DX11Common::GetContext()->CSSetShader(m_ComputeShader, nullptr, 0);
	}

	void DX11Shader::Reload() {
		Load();
	}

	const STL::string& DX11Shader::GetShaderSource(const ShaderType& type) const {
		return m_ShaderSourceMap.at(type);
	}

	STL::unordered_map<ShaderType, STL::string> DX11Shader::PreprocessSource(const STL::string& source) {
		STL::unordered_map<ShaderType, STL::string> shaderSources;

		const char* typeToken = "#type";
		ULLong typeTokenLength = strlen(typeToken);
		ULLong pos = source.find(typeToken, 0); // Start of shader type declaration file;

		while (pos != STL::string::npos)
		{
			ULLong eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
			ST_ENGINE_ASSERT(eol != STL::string::npos, "Syntax error");
			ULLong begin = pos + typeTokenLength + 1; // Start of shader type name (after "#type" keyword)
			STL::string type = source.substr(begin, eol - begin);
			ST_ENGINE_ASSERT((Int)Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			ULLong nextLinePos = source.find_first_not_of("\r\n", eol); // Start of shader code after shader type declaration line
			ST_ENGINE_ASSERT(nextLinePos != STL::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] =
				(pos == STL::string::npos) ? source.substr(nextLinePos) :
				source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void DX11Shader::CompileFromSource() {
		HRESULT result;
		ID3DBlob* errorMessages;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

	#ifdef ST_DEBUG
		flags |= D3DCOMPILE_DEBUG;
	#endif // ST_DEBUG

		for (auto& tuple : m_ShaderSourceMap)
		{
			ShaderType type = tuple.first;
			const STL::string& shader = tuple.second;

			result = D3DCompile(shader.c_str(), shader.size(), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				Utils::EntryPointFromShaderType(type), Utils::ProfileFromShaderType(type), flags, 0,
				&m_ShaderBinaryMap[type], &errorMessages);

		#if ST_DEBUG
			if (FAILED(result))
			{
				char* error = (char*)errorMessages->GetBufferPointer();
				error[strnlen_s(error, 128) - 1] = '\0';

				ST_ENGINE_ERROR("{0}", error);
				errorMessages->Release();
				ST_ENGINE_ERROR("{0}::{1} Shader Compilation error", m_ShaderName.c_str(), Utils::StringFromShaderType(type));
				ST_ENGINE_ASSERT(false, "");
			}
		#endif // ST_DEBUG

			if (errorMessages)
				errorMessages->Release();
		}
	}

	void DX11Shader::Load() {
		Reset();

		STL::string& source = Filesystem::ReadFile(m_FilePath);

		if (source.empty())
		{
			ST_ENGINE_ASSERT("Shader source at path {0} is empty", m_FilePath.c_str());
			return;
		}

		m_ShaderSourceMap = PreprocessSource(source);
		CompileFromSource();

		if (m_ShaderBinaryMap[ShaderType::VERTEX])
		{
			DX11Common::GetDevice()->CreateVertexShader(m_ShaderBinaryMap[ShaderType::VERTEX]->GetBufferPointer(),
				m_ShaderBinaryMap[ShaderType::VERTEX]->GetBufferSize(), nullptr, &m_VertexShader);
		}

		if (m_ShaderBinaryMap[ShaderType::PIXEL])
		{
			DX11Common::GetDevice()->CreatePixelShader(m_ShaderBinaryMap[ShaderType::PIXEL]->GetBufferPointer(),
				m_ShaderBinaryMap[ShaderType::PIXEL]->GetBufferSize(), nullptr, &m_PixelShader);
		}

		if (m_ShaderBinaryMap[ShaderType::COMPUTE])
		{
			DX11Common::GetDevice()->CreateComputeShader(m_ShaderBinaryMap[ShaderType::COMPUTE]->GetBufferPointer(),
				m_ShaderBinaryMap[ShaderType::COMPUTE]->GetBufferSize(), nullptr, &m_ComputeShader);
		}
	}

	void DX11Shader::Reset() {
		for (auto& tuple : m_ShaderBinaryMap)
		{
			if (tuple.second != nullptr)
			{
				tuple.second->Release();
				tuple.second = nullptr;
			}
		}

		m_ShaderBinaryMap.clear();
		m_ShaderSourceMap.clear();

		if (m_VertexShader != nullptr)
		{
			m_VertexShader->Release();
			m_VertexShader = nullptr;
		}

		if (m_PixelShader != nullptr)
		{
			m_PixelShader->Release();
			m_VertexShader = nullptr;
		}

		if (m_ComputeShader != nullptr)
		{
			m_ComputeShader->Release();
			m_ComputeShader = nullptr;
		}
	}
}
