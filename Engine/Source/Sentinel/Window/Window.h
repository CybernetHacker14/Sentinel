#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	template<typename T>
	class WindowCRTP;

	enum class WindowMode {
		WINDOWED = 0,
		WINDOWEDMAXIMIZED = 1,
		FULLSCREEN = 2
	};

	struct WindowProperties {
		STL::string Title;
		UInt32 Width;
		UInt32 Height;
		WindowMode Mode;
		Bool FramebufferTransparency;

		WindowProperties(
			const STL::string& title = "Sentinel-Engine",
			UInt32 width = 900,
			UInt32 height = 900,
			Sentinel::WindowMode mode = WindowMode::WINDOWED,
			Bool transparency = false)
			:Title(title), Width(width), Height(height),
			Mode(mode), FramebufferTransparency(transparency) {}
	};

	// Interface representing a desktop system based Window
	class Window {
	public:
		using EventCallbackFn = STL::function<void(UniqueRef<Event>)>;

		void OnUpdate();

		UInt32 GetWidth();
		UInt32 GetHeight();

		void SetEventCallback(const EventCallbackFn& callback);
		void SetVSync(Bool enabled);
		Bool IsVSync();

		void* GetNativeWindow();

		static UniqueRef<Window> Create(const WindowProperties& props = WindowProperties());

	protected:
		Window() = default;

	private:
		template<typename T>
		inline WindowCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<WindowCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from WindowCRTP<T>.");
			return static_cast<WindowCRTP<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<WindowCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from WindowCRTP<T>.");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class WindowCRTP : public Window {
	private:
		inline void OnUpdate() {
			underlying().OnUpdate();
		}

		inline UInt32 GetWidth() {
			return underlying().GetWidth();
		}

		inline UInt32 GetHeight() {
			return underlying().GetHeight();
		}

		inline void SetEventCallback(const EventCallbackFn& callback) {
			underlying().SetEventCallback(callback);
		}

		inline void SetVSync(Bool enabled) {
			underlying().SetVSync(enabled);
		}

		inline Bool IsVSync() {
			return underlying().IsVSync();
		}

		inline void* GetNativeWindow() {
			return underlying().GetNativeWindow();
		}

	private:
		friend T;
		friend Window;
		WindowCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
