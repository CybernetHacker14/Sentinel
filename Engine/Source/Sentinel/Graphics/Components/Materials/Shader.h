#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	template<typename T>
	class Shader;

	enum class ShaderType {
		NONE = 0,
		VERTEX = 1,
		PIXEL = 2,
		COMPUTE = 3
	};

	class ShaderBase : public IntrusiveRefObject {
	public:
		template<typename T>
		inline Shader<T>* BaseDowncast() {
			static_assert(STL::is_base_of<Shader<T>, T>::value,
				"Operation not allowed. 'T' should be derived from Shader<T>.");
			return static_cast<Shader<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<Shader<T>, T>::value,
				"Operation not allowed. 'T' should be derived from Shader<T>.");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class Shader : public ShaderBase {
	public:
		inline void Bind() {
			underlying().Bind();
		}

		inline void Reload() {
			underlying().Reload();
		}

		inline const STL::string& GetShaderSource(const ShaderType& type = ShaderType::NONE) const {
			return underlying().GetShaderSource(type);
		}

		inline STL::string GetName() {
			return underlying().GetName();
		}

		inline const STL::string& GetFilepath() const {
			return underlying().GetFilepath();
		}

	private:
		friend T;
		Shader() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};

	class ShaderUtils {
	public:
		static Ref<ShaderBase> Create(const STL::string& filepath, const STL::string& name = "Sentinel Shader");
	};
}
