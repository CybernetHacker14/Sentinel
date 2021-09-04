#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

namespace Sentinel
{
	class DX11Shader : public Shader<DX11Shader> {
	public:
		DX11Shader(const STL::string& filepath);
		~DX11Shader();

		void Bind() const;
		void Reload();
		const STL::string& GetShaderSource(const ShaderType& type) const;

	private:
		ID3D11VertexShader* m_VertexShader;
		ID3D11PixelShader* m_PixelShader;
		ID3D11ComputeShader* m_ComputeShader;

		STL::string m_FilePath;

		STL::unordered_map<ShaderType, ID3DBlob*> m_RawBlobMap;
		STL::unordered_map<ShaderType, STL::string> m_ShaderSourceMap;
	};
}
