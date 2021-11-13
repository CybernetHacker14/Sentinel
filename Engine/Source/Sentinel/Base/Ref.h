#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	class IntrusiveRefObject {
	public:
		inline void AddRef() const {
			m_RefCount++;
		}

		inline void ReleaseRef() const {
			m_RefCount--;
		}

		inline void Reset() const {
			m_RefCount = 0;
		}

		inline UInt32 GetRefCount() const { return m_RefCount; }

		inline bool ShouldBeFreed() const { return m_RefCount == 0; }

	private:
		mutable UInt32 m_RefCount = 0;
	};

	template<typename T>
	class IntrusiveRefHandle {
	public:
		IntrusiveRefHandle()
			:m_IntrusiveRefObject(nullptr) {}

		IntrusiveRefHandle(std::nullptr_t ptr)
			:m_IntrusiveRefObject(nullptr) {}

		IntrusiveRefHandle(T* refObject)
			:m_IntrusiveRefObject(refObject) {
			static_assert(STL::is_base_of<IntrusiveRefObject, T>::value,
				"Non IntrusiveRefObject detected");
			AddRef();
		}

		template<typename U>
		IntrusiveRefHandle(const IntrusiveRefHandle<U>& other) {
			m_IntrusiveRefObject = (T*)other.m_IntrusiveRefObject;
			AddRef();
		}

		template<typename U>
		IntrusiveRefHandle(IntrusiveRefHandle<U>&& other) {
			m_IntrusiveRefObject = (T*)other.m_IntrusiveRefObject;
			other.m_IntrusiveRefObject = nullptr;
		}

		~IntrusiveRefHandle() {
			ReleaseRef();
		}

		IntrusiveRefHandle(const IntrusiveRefHandle<T>& other)
			:m_IntrusiveRefObject(other.m_IntrusiveRefObject) {
			AddRef();
		}

		IntrusiveRefHandle& operator=(std::nullptr_t) {
			ReleaseRef();
			m_IntrusiveRefObject = nullptr;
			return *this;
		}

		IntrusiveRefHandle& operator=(const IntrusiveRefHandle<T>& other) {
			other.AddRef();
			ReleaseRef();

			m_IntrusiveRefObject = other.m_IntrusiveRefObject;
			return *this;
		}

		template<typename U>
		IntrusiveRefHandle& operator=(const IntrusiveRefHandle<U>& other) {
			other.AddRef();
			ReleaseRef();

			m_IntrusiveRefObject = other.m_IntrusiveRefObject;
			return *this;
		}

		template<typename U>
		IntrusiveRefHandle& operator=(IntrusiveRefHandle<U>&& other) {
			ReleaseRef();

			m_IntrusiveRefObject = other.m_IntrusiveRefObject;
			other.m_IntrusiveRefObject = nullptr;
			return *this;
		}

		operator bool() { return m_IntrusiveRefObject != nullptr; }
		operator bool() const { return m_IntrusiveRefObject != nullptr; }

		T* operator->() { return m_IntrusiveRefObject; }
		const T* operator->() const { return m_IntrusiveRefObject; }

		T* get() { return m_IntrusiveRefObject; }
		const T* get() const { return m_IntrusiveRefObject; }

	public:
		template<typename... Args>
		static IntrusiveRefHandle<T> CreateRefHandle(Args&&... args) {
			return IntrusiveRefHandle<T>(new T(STL::forward<Args>(args)...));
		}

	private:
		void AddRef() const {
			if (m_IntrusiveRefObject)
			{
				m_IntrusiveRefObject->AddRef();
			}
		}

		void ReleaseRef() const {
			if (m_IntrusiveRefObject)
			{
				m_IntrusiveRefObject->ReleaseRef();
				if (m_IntrusiveRefObject->ShouldBeFreed())
				{
					delete m_IntrusiveRefObject;
				}
			}
		}
	private:
		template<typename U>
		friend class IntrusiveRefHandle;

		T* m_IntrusiveRefObject;
	};
}
