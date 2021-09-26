#pragma once

// Preprocessor directive defined here, because then it needs to be
// entered in all premake and cmake scripts for the definition to get evaluated
#define USE_EASTL 0

// Includes don't have or need alternate implementations can be put here
#include <initializer_list>

#if USE_EASTL == 1
#undef EASTL_EASTDC_VSNPRINTF
#define EASTL_EASTDC_VSNPRINTF 0
#endif // USE_EASTL == 1

#if USE_EASTL == 1
#include <EASTL/vector.h>
#include <EASTL/string.h>
#include <EASTL/unordered_map.h>
#include <EASTL/algorithm.h>
#include <EASTL/array.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/list.h>
#include <EASTL/unordered_set.h>
#include <EASTL/deque.h>
#include <EASTL/algorithm.h>
#include <EASTL/type_traits.h>
#include <EASTL/utility.h>
#include <EASTL/functional.h>
#else
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <memory>
#include <list>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <type_traits>
#include <utility>
#include <functional>
#include <utility>
#endif // USE_EASTL

// The purpose of this header file is to provide an abstraction between types like string, vector, etc.
// so that we can replace the underlying type with std version or eastl version

namespace Sentinel
{
	// Namespace to switch C++ STL with other STLs
	namespace STL
	{
		template<typename T>
		using initializer_list = std::initializer_list<T>;

	#if USE_EASTL == 1

		template<typename T>
		using vector = eastl::vector<T>;

		template<typename T1, typename T2>
		using unordered_map = eastl::unordered_map<T1, T2>;

		using string = eastl::string;

		using wstring = eastl::wstring;

		using string_view = eastl::string_view;

		template<typename T>
		using shared_ptr = eastl::shared_ptr<T>;

		template<typename T, typename ... Args>
		inline shared_ptr<T> make_shared(Args&&... args) {
			return static_cast<shared_ptr<T>>(eastl::make_shared<T>(eastl::forward<Args>(args)...));
		}

		template<typename T>
		using unique_ptr = eastl::unique_ptr<T>;

		template<typename T, typename ... Args>
		inline unique_ptr<T> make_unique(Args&&... args) {
			return static_cast<unique_ptr<T>>(eastl::make_unique<T>(eastl::forward<Args>(args)...));
		}

		template<typename T>
		using list = eastl::list<T>;

		template<typename T>
		using unordered_set = eastl::unordered_set<T>;

		template<typename T>
		using deque = eastl::deque<T>;

		template<typename InputIterator, typename T>
		inline InputIterator find(InputIterator first, InputIterator last, const T& value) {
			return eastl::find(first, last, value);
		}

		template<typename T>
		using underlying_type_t = eastl::underlying_type_t<T>;

		template<typename T>
		inline T&& forward(typename eastl::remove_reference<T>::type& x) {
			return static_cast<T&&>(eastl::forward<T>(x));
		}

		template<typename T>
		inline T&& forward(typename eastl::remove_reference<T>::type&& x) {
			return static_Cast<T&&>(eastl::forward<T>(x));
		}

		template<typename Container>
		inline auto begin(Container& container) -> decltype(container.begin()) {
			return eastl::begin(container);
		}

		template<typename Container>
		inline auto end(Container& container) -> decltype(container.end()) {
			return eastl::end(container);
		}

		template<typename T>
		inline typename eastl::remove_reference<T>::type&& move(T&& x) {
			return eastl::move(x);
		}

		template<typename T>
		using function = eastl::function<T>;

		template<typename T1, typename T2>
		using is_base_of = eastl::is_base_of<T1, T2>;

		template<typename T1, typename T2>
		using pair = eastl::pair<T1, T2>;

		inline string to_string(int value) {
			return eastl::to_string(value);
		}
		inline string to_string(long value) {
			return eastl::to_string(value);
		}
		inline string to_string(long long value) {
			return eastl::to_string(value);
		}
		inline string to_string(unsigned value) {
			return eastl::to_string(value);
		}
		inline string to_string(unsigned long value) {
			return eastl::to_string(value);
		}
		inline string to_string(unsigned long long value) {
			return eastl::to_string(value);
		}
		inline string to_string(float value) {
			return eastl::to_string(value);
		}
		inline string to_string(double value) {
			return eastl::to_string(value);
		}
		inline string to_string(long double value) {
			return eastl::to_string(value);
		}

		inline wstring to_wstring(int value) {
			return eastl::to_wstring(value);
		}
		inline wstring to_wstring(long value) {
			return eastl::to_wstring(value);
		}
		inline wstring to_wstring(long long value) {
			return eastl::to_wstring(value);
		}
		inline wstring to_wstring(unsigned value) {
			return eastl::to_wstring(value);
		}
		inline wstring to_wstring(unsigned long value) {
			return eastl::to_wstring(value);
		}
		inline wstring to_wstring(unsigned long long value) {
			return eastl::to_wstring(value);
		}
		inline wstring to_wstring(float value) {
			return eastl::to_wstring(value);
		}
		inline wstring to_wstring(double value) {
			return eastl::to_wstring(value);
		}
		inline wstring to_wstring(long double value) {
			return eastl::to_wstring(value);
		}

		template<typename T, typename U>
		using is_same = eastl::is_same<T, U>;

		template<typename T, typename U>
		constexpr bool is_same_v = eastl::is_same_v<T, U>;

	#else

		template<typename T>
		using vector = std::vector<T>;

		template<typename T1, typename T2>
		using unordered_map = std::unordered_map<T1, T2>;

		using string = std::string;

		using wstring = std::wstring;

		using string_view = std::string_view;

		template<typename T>
		using shared_ptr = std::shared_ptr<T>;

		template<typename T, typename ... Args>
		inline shared_ptr<T> make_shared(Args&&... args) {
			return static_cast<shared_ptr<T>>(std::make_shared<T>(std::forward<Args>(args)...));
		}

		template<typename T>
		using unique_ptr = std::unique_ptr<T>;

		template<typename T, typename ... Args>
		inline unique_ptr<T> make_unique(Args&&... args) {
			return static_cast<unique_ptr<T>>(std::make_unique<T>(std::forward<Args>(args)...));
		}

		template<typename T>
		using list = std::list<T>;

		template<typename T>
		using unordered_set = std::unordered_set<T>;

		template<typename T>
		using deque = std::deque<T>;

		template<typename InputIterator, typename T>
		inline InputIterator find(InputIterator first, InputIterator last, const T& value) {
			return std::find(first, last, value);
		}

		template<typename T>
		using underlying_type_t = std::underlying_type_t<T>;

		template<typename T>
		inline T&& forward(typename std::remove_reference<T>::type& x) {
			return std::forward<T>(x);
		}

		template<typename T>
		inline T&& forward(typename std::remove_reference<T>::type&& x) {
			return std::forward<T>(x);
		}

		template<typename Container>
		inline auto begin(Container& container) -> decltype(container.begin()) {
			return std::begin(container);
		}

		template<typename Container>
		inline auto end(Container& container) -> decltype(container.end()) {
			return std::end(container);
		}

		template<typename T>
		inline typename std::remove_reference<T>::type&& move(T&& x) {
			return std::move(x);
		}

		template<typename T>
		using function = std::function<T>;

		template<typename T1, typename T2>
		using is_base_of = std::is_base_of<T1, T2>;

		template<typename T1, typename T2>
		using pair = std::pair<T1, T2>;

		inline string to_string(int value) {
			return std::to_string(value);
		}
		inline string to_string(long value) {
			return std::to_string(value);
		}
		inline string to_string(long long value) {
			return std::to_string(value);
		}
		inline string to_string(unsigned value) {
			return std::to_string(value);
		}
		inline string to_string(unsigned long value) {
			return std::to_string(value);
		}
		inline string to_string(unsigned long long value) {
			return std::to_string(value);
		}
		inline string to_string(float value) {
			return std::to_string(value);
		}
		inline string to_string(double value) {
			return std::to_string(value);
		}
		inline string to_string(long double value) {
			return std::to_string(value);
		}

		inline wstring to_wstring(int value) {
			return std::to_wstring(value);
		}
		inline wstring to_wstring(long value) {
			return std::to_wstring(value);
		}
		inline wstring to_wstring(long long value) {
			return std::to_wstring(value);
		}
		inline wstring to_wstring(unsigned value) {
			return std::to_wstring(value);
		}
		inline wstring to_wstring(unsigned long value) {
			return std::to_wstring(value);
		}
		inline wstring to_wstring(unsigned long long value) {
			return std::to_wstring(value);
		}
		inline wstring to_wstring(float value) {
			return std::to_wstring(value);
		}
		inline wstring to_wstring(double value) {
			return std::to_wstring(value);
		}
		inline wstring to_wstring(long double value) {
			return std::to_wstring(value);
		}

		template<typename T, typename U>
		using is_same = std::is_same<T, U>;

		template<typename T, typename U>
		constexpr bool is_same_v = std::is_same_v<T, U>;

	#endif // USE_EASTL
	}
}
