#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

namespace Sentinel
{
	template<typename T>
	class Texture2DCRTP;

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
		void Bind(UInt32 slot, const ShaderType shaderType);
		void Unbind(UInt32 slot, const ShaderType shaderType);

		Bool IsHDR();
		Bool IsLoaded();

		UInt32 GetWidth();
		UInt32 GetHeight();

		static SharedRef<Texture2D> Create(const Texture2DImportSettings& settings);

	protected:
		Texture2D() = default;

	public:
		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<Texture2DCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from Texture2DCRTP<T>.");
			return static_cast<T*>(this);
		}

	private:
		template<typename T>
		inline Texture2DCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<Texture2DCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from Texture2DCRTP<T>.");
			return static_cast<Texture2DCRTP<T>*>(this);
		}
	};

	template<typename T>
	class Texture2DCRTP : public Texture2D {
	private:
		inline void Bind() {
			underlying().Bind();
		}

		inline void Unbind() {
			underlying().Unbind();
		}

		inline Bool IsHDR() {
			return underlying().IsHDR();
		}

		inline Bool IsLoaded() {
			return underlying().IsLoaded();
		}

		inline UInt32 GetWidth() {
			return underlying().GetWdith();
		}

		inline UInt32 GetHeight() {
			return underlying().GetHeight();
		}

	private:
		friend T;
		friend Texture2D;
		Texture2DCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
