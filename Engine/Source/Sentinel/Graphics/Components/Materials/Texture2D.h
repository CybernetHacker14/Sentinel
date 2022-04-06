#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

namespace Sentinel
{
	enum class WrapMode {
		REPEAT = 0,
		CLAMP = 1
	};

	struct Texture2DImportSettings {
		STL::string texturePath;

		WrapMode WrapMode = WrapMode::CLAMP;

		Bool ReadWriteEnabled = false;
		Bool GenerateMipMaps = false;
		Bool sRGB = false;
	};

	class Texture2D : public ISharedRef {
	public:
		inline ~Texture2D() {
			if (!m_DestructorFunction)
				return;

			m_DestructorFunction();
		}

		inline void Bind(UInt32 slot, const ShaderType shaderType) const {
			if (!m_BindFunction)
				return;

			m_BindFunction(slot, shaderType);
		}

		inline void Unbind(UInt32 slot, const ShaderType shaderType) const {
			if (!m_UnbindFunction)
				return;

			m_UnbindFunction(slot, shaderType);
		}

		inline Bool IsHDR() { return m_HDR; }
		inline Bool IsLoaded() { return m_Loaded; }

		inline UInt32 GetWidth() { return m_Width; }
		inline UInt32 GetHeight() { return m_Height; }

	public:
		static SharedRef<Texture2D> Create(const Texture2DImportSettings& settings);

	public:
		template<typename T>
		inline T* Cast() {
			static_assert(STL::is_base_of<Texture2D, T>::value,
				"'T' should be a derived from Texture2D.");
			return static_cast<T*>(this);
		}

	protected:
		Texture2D(const Texture2DImportSettings& settings);

	protected:
		STL::delegate<void(UInt32, const ShaderType)> m_BindFunction;
		STL::delegate<void(UInt32, const ShaderType)> m_UnbindFunction;
		STL::delegate<void()> m_DestructorFunction;

	protected:
		void* m_TexturePixels = nullptr;
		Bool m_Loaded = false, m_HDR = false;
		UInt32 m_Width = 0, m_Height = 0;
		Texture2DImportSettings m_Settings;
	};
}
