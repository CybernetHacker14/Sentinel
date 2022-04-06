#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	class Constantbuffer : public ISharedRef {
	public:
		enum class UsageType {
			DEFAULT = 0,
			DYNAMIC = 1
		};

	public:
		inline ~Constantbuffer() {
			if (!m_DestructorFunction)
				return;

			m_DestructorFunction();
		}

		inline void VSBind() const {
			if (!m_VSBindFunction)
				return;

			m_VSBindFunction();
		}

		inline void PSBind() const {
			if (!m_PSBindFunction)
				return;

			m_PSBindFunction();
		}

		inline void CSBind() const {
			if (!m_CSBindFunction)
				return;

			m_CSBindFunction();
		}

		inline void SetStaticData(void* data) const {
			if (!m_SetStaticDataFunction)
				return;

			m_SetStaticDataFunction(data);
		}

		inline void SetDynamicData(void* data) const {
			if (!m_SetDynamicDataFunction)
				return;

			m_SetDynamicDataFunction(data);
		}

		inline const void* GetData() const { return m_Data; }
		inline const UInt32 GetSize() const { return m_Size; }
		inline const UsageType GetUsageTypeFunction() const { return m_UsageType; }

	public:
		static SharedRef<Constantbuffer> Create(UInt32 size, UInt32 bindSlot, UsageType usageType);

	protected:
		Constantbuffer(UInt32 size, UInt32 bindSlot, UsageType usageType);

	protected:
		STL::delegate<void()> m_VSBindFunction;
		STL::delegate<void()> m_PSBindFunction;
		STL::delegate<void()> m_CSBindFunction;

		STL::delegate<void(void*)> m_SetStaticDataFunction;
		STL::delegate<void(void*)> m_SetDynamicDataFunction;

		STL::delegate<void()> m_DestructorFunction;

	protected:
		void* m_Data = nullptr;
		UInt32 m_Size = 0;
		UsageType m_UsageType = UsageType::DEFAULT;
	};
}
