#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	template<typename T>
	class Texture2DCRTP;

	enum class WrapMode {
		REPEAT = 0,
		CLAMP = 1
	};

	struct Texture2DImportSettings {
		std::filesystem::path TexturePath = std::filesystem::path();

		WrapMode WrapMode = WrapMode::CLAMP;

		Bool ReadWriteEnabled = false;
		Bool GenerateMipMaps = false;
		Bool sRGB = false;
	};

	class Texture2D : public ISharedRef {
	public:
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


	private:
		friend T;
		friend Texture2D;
		Texture2DCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
