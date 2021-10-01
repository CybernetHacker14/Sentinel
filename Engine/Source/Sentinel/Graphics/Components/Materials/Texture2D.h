#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	template<typename T>
	class Texture2DCRTP;

	struct Texture2DImportSpecifications {
		std::filesystem::path TexturePath;

		Bool ReadWriteEnabled;
		Bool GenerateMipMaps;
		Bool IsHDR;
		Bool sRGB;
	};

	class Texture2D : public IntrusiveRefObject {
	public:
		static Ref<Texture2D> Create();

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
