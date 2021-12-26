#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/System/UUID.h"
#include "Sentinel/AssetManagement/AssetInfo.h"

#pragma warning(push, 0)
#include <cereal/archives/xml.hpp>
#pragma warning(pop)

namespace Sentinel
{
	class Asset : public ISharedRef {
	public:
		Asset(const STL::string& filename, const STL::string& extension, const STL::string& virtualFilepath, const AssetType& type = AssetType::NONE)
			: m_AssetFilename(filename), m_AssetFileExtension(extension), m_AssetVirtualFilepath(virtualFilepath), m_AssetType(type) {}

		inline const STL::string& GetFilename() const { return m_AssetFilename; }
		inline const STL::string& GetFileExtension() const { return m_AssetFileExtension; }
		inline const STL::string& GetVirtualFilepath() const { return m_AssetVirtualFilepath; }

		inline const UUID& GetAssetUUID() const { return m_AssetUUID; }
		inline const AssetType& GetAssetType() const { return m_AssetType; }

		template<class Archive>
		void save(Archive& archive) const {
			archive(m_AssetUUID, m_AssetType, m_AssetFilename, m_AssetFileExtension, m_AssetFileExtension);
		}

		template<class Archive>
		void load(Archive& archive) {
			archive(m_AssetUUID, m_AssetType, m_AssetFilename, m_AssetFileExtension, m_AssetFileExtension);
		}

	private:
		STL::string m_AssetFilename;
		STL::string m_AssetFileExtension;
		STL::string m_AssetVirtualFilepath;

		UUID m_AssetUUID = 0;
		AssetType m_AssetType;
	};
}
