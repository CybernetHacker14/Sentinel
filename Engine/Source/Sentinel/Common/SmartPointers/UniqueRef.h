#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel
{
#if true
	template<typename T>
	class UniqueRef {
	public:
		UniqueRef()
			:m_UniqueRefObject(nullptr) {}

		UniqueRef(T* other)
			:m_UniqueRefObject(other) {}

		UniqueRef(std::nullptr_t ptr)
			:m_UniqueRefObject(nullptr) {}

		~UniqueRef() {
			if (m_UniqueRefObject)
				delete m_UniqueRefObject;
		}

		UniqueRef(const UniqueRef&) = delete;
		UniqueRef& operator=(const UniqueRef&) = delete;

		template<typename U>
		UniqueRef(UniqueRef<U>&& other) {
			m_UniqueRefObject = (T*)other.m_UniqueRefObject;
			other.m_UniqueRefObject = nullptr;
		}

		template<typename U>
		UniqueRef& operator=(UniqueRef<U>&& other) {
			m_UniqueRefObject = (T*)other.m_UniqueRefObject;
			other.m_UniqueRefObject = nullptr;
			return *this;
		}

		Bool operator==(const UniqueRef<T>& other) const {
			return m_UniqueRefObject == other.m_UniqueRefObject;
		}

		Bool operator!=(const UniqueRef<T>& other) const {
			return !(*this == other);
		}

		operator bool() { return m_UniqueRefObject != nullptr; }
		operator bool() const { return m_UniqueRefObject != nullptr; }

		T* operator->() { return m_UniqueRefObject; }
		const T* operator->() const { return m_UniqueRefObject; }

		T& operator*() { return *m_UniqueRefObject; }
		const T& operator*() const { return *m_UniqueRefObject; }

		T* get() { return m_UniqueRefObject; }
		const T* get() const { return m_UniqueRefObject; }

	public:
		template<typename... Args>
		static UniqueRef<T> CreateRef(Args&&... args) {
			return UniqueRef<T>(new T(STL::forward<Args>(args)...));
		}

	private:
		template<typename U>
		friend class UniqueRef;

		T* m_UniqueRefObject;
	};

	template<typename T, typename... Args>
	constexpr UniqueRef<T> CreateUniqueRef(Args&&... args) {
		return UniqueRef<T>::CreateRef(args...);
	}
#else
	template<typename T>
	using UniqueRef = STL::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr UniqueRef<T> CreateUniqueRef(Args&&... args) {
		return STL::make_unique<T>(STL::forward<Args>(args)...);
	}
#endif
}
