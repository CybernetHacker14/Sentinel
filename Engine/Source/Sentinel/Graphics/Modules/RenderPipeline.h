#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Definitions/FrameBackings.h"
#include "Sentinel/Graphics/Definitions/RenderResources.h"

namespace Sentinel
{
	class RenderPipeline {
	private:
		inline void CreateFrameBackings(const WindowProperties& windowProps,
			const FramebufferSpecification& framebufferSpecs) {
			if (!m_CreateFrameBackingsFunction)
				return;

			m_CreateFrameBackingsFunction(windowProps, framebufferSpecs);
		}

		inline void CreateFrameBackingsComponents() {
			if (!m_CreateFrameBackingsComponentsFunction)
				return;

			m_CreateFrameBackingsComponentsFunction();
		}

		inline void InitFrameBackings() {
			if (!m_InitFrameBackingsFunction)
				return;

			m_InitFrameBackingsFunction();
		}

		inline void BindRenderResources() {
			if (!m_BindRenderResourcesFunction)
				return;

			m_BindRenderResourcesFunction();
		}

		inline void BindFramebuffer() {
			if (!m_BindFramebufferFunction)
				return;

			m_BindFramebufferFunction();
		}

		inline void Resize(UInt32 width, UInt32 height) {
			if (!m_ResizeFunction)
				return;

			m_ResizeFunction(width, height);
		}

		inline void Draw() {
			if (!m_DrawFunction)
				return;

			m_DrawFunction();
		}

		inline void SwapBuffers(Bool vSync = true) {
			if (!m_SwapBuffersFunction)
				return;

			m_SwapBuffersFunction(vSync);
		}

		inline void Clear() {
			if (!m_ClearFunction)
				return;

			m_ClearFunction();
		}

		inline void UnbindFramebuffer() {
			if (!m_UnbindFramebufferFunction)
				return;

			m_UnbindFramebufferFunction();
		}

		inline void UnbindRenderResources() {
			if (!m_UnbindRenderResourcesFunction)
				return;

			m_UnbindRenderResourcesFunction();
		}

		inline void CleanRenderResources() {
			if (!m_CleanRenderResourcesFunction)
				return;

			m_CleanRenderResourcesFunction();
		}

		inline void CleanFrameBackings() {
			if (!m_CleanFrameBackingsFunction)
				return;

			m_CleanFrameBackingsFunction();
		}

		inline void CleanNativeResources() {
			if (!m_CleanNativeResourcesFunction)
				return;

			m_CleanNativeResourcesFunction();
		}

	private:
		static UniqueRef<RenderPipeline> Create();

	protected:
		STL::delegate<void(const WindowProperties&,
			const FramebufferSpecification&)> m_CreateFrameBackingsFunction;
		STL::delegate<void()> m_CreateFrameBackingsComponentsFunction;
		STL::delegate<void()> m_InitFrameBackingsFunction;
		STL::delegate<void()> m_BindRenderResourcesFunction;
		STL::delegate<void()> m_BindFramebufferFunction;

		STL::delegate<void(UInt32, UInt32)> m_ResizeFunction;
		STL::delegate<void()> m_DrawFunction;
		STL::delegate<void(Bool)> m_SwapBuffersFunction;
		STL::delegate<void()> m_ClearFunction;

		STL::delegate<void()> m_UnbindFramebufferFunction;
		STL::delegate<void()> m_UnbindRenderResourcesFunction;
		STL::delegate<void()> m_CleanRenderResourcesFunction;
		STL::delegate<void()> m_CleanFrameBackingsFunction;
		STL::delegate<void()> m_CleanNativeResourcesFunction;

	protected:
		friend class Renderer;

		SharedRef<FrameBackings> m_FrameBackings;
		SharedRef<RenderResources> m_RenderResources;
	};
}
