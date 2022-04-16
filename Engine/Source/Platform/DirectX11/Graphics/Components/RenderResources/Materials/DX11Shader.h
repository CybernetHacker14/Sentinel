#pragma once

#include "Sentinel/Graphics/Components/RenderResources/Materials/Shader.h"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

namespace Sentinel
{
	class DX11Shader final : public Shader {
	public:
		DX11Shader(const STL::string& filepath, const STL::string& name);

		ID3DBlob* GetVertexShaderBinary() const { return m_ShaderBinaryMap.at(ShaderType::VERTEX); }
		ID3DBlob* GetPixelShaderBinary() const { return m_ShaderBinaryMap.at(ShaderType::PIXEL); }
		ID3DBlob* GetComputeShaderBinary() const { return m_ShaderBinaryMap.at(ShaderType::COMPUTE); }

	private:
		void Bind() const;
		void Reload();

		STL::unordered_map<ShaderType, STL::string> PreprocessSource(const STL::string& source);

		void CompileFromSource();
		void Load();
		void Reset();

		void Clean();

	private:
		STL::unordered_map<ShaderType, ID3DBlob*> m_ShaderBinaryMap;

		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;
		ID3D11ComputeShader* m_ComputeShader = nullptr;
	};
}
