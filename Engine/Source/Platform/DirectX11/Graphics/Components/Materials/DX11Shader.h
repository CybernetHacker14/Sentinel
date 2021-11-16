#pragma once

#include "Sentinel/Graphics/Components/Materials/Shader.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Shader final : public ShaderCRTP<DX11Shader> {
	public:
		DX11Shader(const std::filesystem::path& filepath, const STL::string& name);
		~DX11Shader();

		ID3DBlob* GetVertexShaderBinary() const { return m_ShaderBinaryMap.at(ShaderType::VERTEX); }
		ID3DBlob* GetPixelShaderBinary() const { return m_ShaderBinaryMap.at(ShaderType::PIXEL); }
		ID3DBlob* GetComputeShaderBinary() const { return m_ShaderBinaryMap.at(ShaderType::COMPUTE); }

	private:
		void Bind() const;
		void Reload();

		inline const STL::string& GetShaderSource(const ShaderType& type) const { return m_ShaderSourceMap.at(type); }
		inline const STL::string& GetName() const { return m_ShaderName; }
		inline const std::filesystem::path& GetFilepath() const { return m_FilePath; }

		STL::unordered_map<ShaderType, STL::string> PreprocessSource(const STL::string& source);

		void CompileFromSource();
		void Load();
		void Reset();

	private:
		template<typename T>
		friend class ShaderCRTP;

		STL::string m_ShaderName;

		std::filesystem::path m_FilePath;

		STL::unordered_map<ShaderType, STL::string> m_ShaderSourceMap;
		STL::unordered_map<ShaderType, ID3DBlob*> m_ShaderBinaryMap;

		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;
		ID3D11ComputeShader* m_ComputeShader = nullptr;
	};
}
