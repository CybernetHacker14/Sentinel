#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	template<typename T>
	class ShaderCRTP;

	enum class ShaderType {
		NONE = 0,
		VERTEX = 1,
		PIXEL = 2,
		COMPUTE = 3
	};

	class Shader : public ISharedRef {
	public:
		void Bind();
		void Reload();

		const STL::string& GetShaderSource(const ShaderType& type = ShaderType::NONE);
		const STL::string& GetName();

		const STL::string& GetFilepath();

		static SharedRef<Shader> Create(const STL::string& filepath, const STL::string& name = "Sentinel Shader");

	protected:
		Shader() = default;

	public:
		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<ShaderCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from ShaderCRTP<T>.");
			return static_cast<T*>(this);
		}

	private:
		template<typename T>
		inline ShaderCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<ShaderCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from ShaderCRTP<T>.");
			return static_cast<ShaderCRTP<T>*>(this);
		}
	};

	template<typename T>
	class ShaderCRTP : public Shader {
	private:
		inline void Bind() {
			underlying().Bind();
		}

		inline void Reload() {
			underlying().Reload();
		}

		inline const STL::string& GetShaderSource(const ShaderType& type) {
			return underlying().GetShaderSource(type);
		}

		inline const STL::string& GetName() {
			return underlying().GetName();
		}

		inline const STL::string& GetFilepath() {
			return underlying().GetFilepath();
		}

	private:
		friend T;
		friend Shader;
		ShaderCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
