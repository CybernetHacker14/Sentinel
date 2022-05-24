#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	class Shader : public ISharedRef {
	public:
		inline void Bind() const {
			if (!m_BindFunction)
				return;

			m_BindFunction();
		}

		inline void Reload() const {
			if (!m_ReloadFunction)
				return;

			m_ReloadFunction();
		}

		inline void Clean() const {
			if (!m_CleanFunction)
				return;

			m_CleanFunction();
		}

		inline const STL::string& GetShaderSource(
			const ShaderType& type = ShaderType::NONE) const {

			return m_ShaderSourceMap.at(type);
		}

		inline const STL::string& GetName() const {
			return m_ShaderName;
		};

		inline const STL::string& GetFilepath() const {
			return m_FilePath;
		};

	public:
		template<typename T>
		inline T* Cast() {
			static_assert(STL::is_base_of<Shader, T>::value,
				"'T' should be a derived from Shader.");
			return static_cast<T*>(this);
		}

	public:
		static SharedRef<Shader> Create(const STL::string& filepath,
			const STL::string& name = "Sentinel Shader");

	protected:
		Shader(const STL::string& filepath, const STL::string& name);

	protected:
		STL::delegate<void()> m_BindFunction;
		STL::delegate<void()> m_ReloadFunction;
		STL::delegate<void()> m_CleanFunction;

	protected:
		STL::string m_ShaderName;
		STL::string m_FilePath;

		STL::unordered_map<ShaderType, STL::string> m_ShaderSourceMap;
	};
}
