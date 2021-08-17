#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	class IntrusiveRefObject {
	public:
		void AddRef() const {
			m_RefCount++;
		}

		void ReleaseRef() const {
			m_RefCount--;
		}

		UInt GetRefCount() const { return m_RefCount; }

		bool ShouldBeFreed() const { return m_RefCount == 0; }

	private:
		mutable UInt m_RefCount = 0;
	};

	template<typename T>
	class IntrusiveRefHandle {
	protected:
		using this_type = IntrusiveRefHandle<T>;

	public:
		using element_type = T;

	public:
		IntrusiveRefHandle()
			:m_IntrusiveRefObject(nullptr) {}

		IntrusiveRefHandle(std::nullptr_t null)
			:m_IntrusiveRefObject(nullptr) {}

		IntrusiveRefHandle(T* instance)
			:m_IntrusiveRefObject(instance) {
			static_assert(STL::is_base_of<IntrusiveRefObject, T>::value,
				"Non IntrusiveRefObject class detected!!!");

			if (m_IntrusiveRefObject)
			{
				AddRef();
			}
		}

		// Copy constructor
		IntrusiveRefHandle(const IntrusiveRefHandle& handle)
			:m_IntrusiveRefObject(handle.m_IntrusiveRefObject) {

			if (m_IntrusiveRefObject)
			{
				AddRef();
			}
		}

		// Move constructor
		IntrusiveRefHandle(IntrusiveRefHandle&& handle)
			: m_IntrusiveRefObject(nullptr) {
			swap(handle);
		}

		// Constructor which copies a pointer from another IntrusiveRefHandle
		template<typename U>
		IntrusiveRefHandle(const IntrusiveRefHandle<U>& handle)
			: m_IntrusiveRefObject(handle.m_IntrusiveRefObject) {
			if (m_IntrusiveRefObject)
				ReleaseRef();
		}

		// Destructor
		~IntrusiveRefHandle() {
			if (m_IntrusiveRefObject)
			{
				ReleaseRef();
			}
		}

		// Copy assignment operator
		IntrusiveRefHandle& operator=(const IntrusiveRefHandle& handle) {
			return operator=(handle.m_IntrusiveRefObject);
		}

		// Move assignemnt operator
		IntrusiveRefHandle& operator=(IntrusiveRefHandle&& handle) {
			swap(handle);
			return *this;
		}

		template <typename U>
		IntrusiveRefHandle& operator=(const IntrusiveRefHandle<U>& handle) {
			return operator=(handle.m_IntrusiveRefObject);
		}

		// The custom overloaded assignment operator
		IntrusiveRefHandle& operator=(T* refObject) {
			static_assert(STL::is_base_of<IntrusiveRefObject, T>::value,
				"Non IntrusiveRefObject class detected!!!");

			if (refObject != m_IntrusiveRefObject)
			{
				T* const pTemp = m_IntrusiveRefObject; // Create temp to prevent possible problems with re-entrancy.
				if (refObject)
					refObject->AddRef();

				m_IntrusiveRefObject = refObject;
				if (pTemp)
					pTemp->ReleaseRef();
			}

			return *this;
		}

		// Returns a reference to the contained object
		T& operator* () const {
			return *m_IntrusiveRefObject;
		}

		// Pointer arrow operator
		T* operator->() const {
			return m_IntrusiveRefObject;
		}

		// Returns a pointer to the contained object
		T* get() const {
			return m_IntrusiveRefObject;
		}

		// Release the owned object and clears our reference to it.
		void reset() {
			static_assert(STL::is_base_of<IntrusiveRefObject, T>::value,
				"Non IntrusiveRefObject class detected!!!");

			T* const pTemp = m_IntrusiveRefObject;
			m_IntrusiveRefObject = nullptr;
			if (pTemp)
				pTemp->ReleaseRef();
		}

		// Exchanges the owned pointer between two IntrusiveRefObjects.
		void swap(this_type& handle) {
			static_assert(STL::is_base_of<IntrusiveRefObject, T>::value,
				"Non IntrusiveRefObject class detected!!!");

			T* const pTemp = m_IntrusiveRefObject;
			m_IntrusiveRefObject = handle.m_IntrusiveRefObject;
			handle.m_IntrusiveRefObject = pTemp;
		}

		// Sets an IntrusiveRefObject without calling AddRef() on the pointed object.
		void attach(T* refObject) {
			static_assert(STL::is_base_of<IntrusiveRefObject, T>::value,
				"Non IntrusiveRefObject class detected!!!");

			T* const pTemp = m_IntrusiveRefObject;
			m_IntrusiveRefObject = refObject;
			if (pTemp)
				pTemp->ReleaseRef();
		}

		// Surrenders the reference held by an IntrusiveRefHandle pointer
		T* detach() {
			static_assert(STL::is_base_of<IntrusiveRefObject, T>::value,
				"Non IntrusiveRefObject class detected!!!");

			T* const pTemp = m_IntrusiveRefObject;
			m_IntrusiveRefObject = nullptr;
			return pTemp;
		}

		typedef T* (this_type::* bool_) () const;

		// Implicit operator bool
		// Note that we do not use the operator bool(). The reason for this
		// is that booleans automatically convert up to short, int, float, etc.
		// The result is that this: if(IntrusiveRefHandle == 1) would yeild true (bad).
		operator bool_() const {
			if (m_IntrusiveRefObject)
				return &this_type::get;

			return nullptr;
		}

		// This return the opposite of operator bool; it return true if
		// the owned pointer is null.
		bool operator!() const {
			return (m_IntrusiveRefObject == nullptr);
		}

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

	template<typename T>
	inline T* GetRawPointerFromIntrusiveRefHandle(const IntrusiveRefHandle<T>& handle) {
		return handle.get();
	}

	template<typename T>
	inline void Swap(IntrusiveRefHandle<T>& handle1, IntrusiveRefHandle<T>& handle2) {
		handle1.swap(handle2);
	}

	template<typename T, typename U>
	bool operator==(IntrusiveRefHandle<T> const& handle1, IntrusiveRefHandle<U> const& handle2) {
		return (handle1.get() == handle2.get());
	}

	template<typename T, typename U>
	bool operator!=(IntrusiveRefHandle<T> const& handle1, IntrusiveRefHandle<U> const& handle2) {
		return (handle1.get() != handle2.get());
	}

	template<typename T>
	bool operator==(IntrusiveRefHandle<T> const& handle, T* rawPointer) {
		return (handle.get() == rawPointer);
	}

	template<typename T>
	bool operator!=(IntrusiveRefHandle<T> const& handle, T* rawPointer) {
		return (handle.get() != rawPointer);
	}

	template<typename T>
	bool operator==(T* rawPointer, IntrusiveRefHandle<T> const& handle) {
		return (rawPointer == handle.get());
	}

	template<typename T>
	bool operator!=(T* rawPointer, IntrusiveRefHandle<T> const& handle) {
		return (rawPointer != handle.get());
	}

	template<class T, class U>
	IntrusiveRefHandle<T> static_handle_cast(const IntrusiveRefHandle<U>& handle) {
		return static_cast<T*>(handle.get());
	}

	template<class T, class U>
	IntrusiveRefHandle<T> dynamic_pointer_cast(const IntrusiveRefHandle<U>& handle) {
		return dynamic_cast<T*>(handle.get());
	}
}
