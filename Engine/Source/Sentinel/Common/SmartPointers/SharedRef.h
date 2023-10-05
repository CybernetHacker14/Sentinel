#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel
{
	class ISharedRef {
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

#if true
	template<typename T>
	class SharedRef {
	public:
		SharedRef()
			:m_SharedRefObject(nullptr) {}

		SharedRef(std::nullptr_t ptr)
			:m_SharedRefObject(nullptr) {}

		SharedRef(T* other)
			:m_SharedRefObject(other) {
			static_assert(STL::is_base_of<ISharedRef, T>::value,
				"Class doesn't inherit ISharedRef interface");
			AddRef();
		}

		template<typename U>
		SharedRef(const SharedRef<U>& other) {
			m_SharedRefObject = (T*)other.m_SharedRefObject;
			AddRef();
		}

		template<typename U>
		SharedRef(SharedRef<U>&& other) {
			m_SharedRefObject = (T*)other.m_SharedRefObject;
			other.m_SharedRefObject = nullptr;
		}

		~SharedRef() {
			ReleaseRef();
		}

		SharedRef(const SharedRef<T>& other)
			:m_SharedRefObject(other.m_SharedRefObject) {
			AddRef();
		}

		SharedRef& operator=(std::nullptr_t) {
			ReleaseRef();
			m_SharedRefObject = nullptr;
			return *this;
		}

		SharedRef& operator=(const SharedRef<T>& other) {
			other.AddRef();
			ReleaseRef();

			m_SharedRefObject = other.m_SharedRefObject;
			return *this;
		}

		template<typename U>
		SharedRef& operator=(const SharedRef<U>& other) {
			other.AddRef();
			ReleaseRef();

			m_SharedRefObject = other.m_SharedRefObject;
			return *this;
		}

		template<typename U>
		SharedRef& operator=(SharedRef<U>&& other) {
			ReleaseRef();

			m_SharedRefObject = other.m_SharedRefObject;
			other.m_SharedRefObject = nullptr;
			return *this;
		}

		Bool operator==(const SharedRef<T>& other) const {
			return m_SharedRefObject == other.m_SharedRefObject;
		}

		Bool operator!=(const SharedRef<T>& other) const {
			return !(*this == other);
		}

		operator bool() { return m_SharedRefObject != nullptr; }
		operator bool() const { return m_SharedRefObject != nullptr; }

		T* operator->() { return m_SharedRefObject; }
		const T* operator->() const { return m_SharedRefObject; }

		T& operator*() { return *m_SharedRefObject; }
		const T& operator*() const { return *m_SharedRefObject; }

		T* get() { return m_SharedRefObject; }
		const T* get() const { return m_SharedRefObject; }

	public:
		template<typename... Args>
		static SharedRef<T> CreateRef(Args&&... args) {
			return SharedRef<T>(new T(std::forward<Args>(args)...));
		}

	private:
		void AddRef() const {
			if (m_SharedRefObject)
			{
				m_SharedRefObject->AddRef();
			}
		}

		void ReleaseRef() const {
			if (m_SharedRefObject)
			{
				m_SharedRefObject->ReleaseRef();
				if (m_SharedRefObject->ShouldBeFreed())
				{
					delete m_SharedRefObject;
				}
			}
		}
	private:
		template<typename U>
		friend class SharedRef;

		T* m_SharedRefObject;
	};

	template<typename T, typename... Args>
	constexpr SharedRef<T> CreateSharedRef(Args&&... args) {
		return SharedRef<T>::CreateRef(args...);
	}
#else
	template<typename T>
	using SharedRef = STL::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr SharedRef<T> CreateSharedRef(Args&&... args) {
		return STL::make_unique<T>(STL::forward<Args>(args)...);
	}
#endif
}
