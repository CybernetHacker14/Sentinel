#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Shader : public Shader<DX11Shader> {
	public:
		DX11Shader(const STL::string& filepath, const STL::string& name);
		~DX11Shader();

		void Bind() const;
		void Reload();
		const STL::string& GetShaderSource(const ShaderType& type) const;

		inline STL::string GetName() { return m_ShaderName; }
		inline const STL::string& GetFilepath() const { return m_FilePath; }

		ID3DBlob* GetVertexShaderBinary() const { return m_ShaderBinaryMap.at(ShaderType::VERTEX); }
		ID3DBlob* GetPixelShaderBinary() const { return m_ShaderBinaryMap.at(ShaderType::PIXEL); }
		ID3DBlob* GetComputeShaderBinary() const { return m_ShaderBinaryMap.at(ShaderType::COMPUTE); }
	private:

		STL::unordered_map<ShaderType, STL::string> PreprocessSource(const STL::string& source);

		void CompileFromSource();

		void Load();
		void Reset();

	private:
		STL::string m_ShaderName;
		STL::string m_FilePath;

		STL::unordered_map<ShaderType, STL::string> m_ShaderSourceMap;
		STL::unordered_map<ShaderType, ID3DBlob*> m_ShaderBinaryMap;

		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;
		ID3D11ComputeShader* m_ComputeShader = nullptr;
	};
}
