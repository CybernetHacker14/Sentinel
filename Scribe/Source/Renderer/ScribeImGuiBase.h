#pragma once

#include <Sentinel.h>

#include <Sentinel/Memory/PoolAllocator.h>

namespace Sentinel {
    struct Texture2DData;
    struct ContextData;
}  // namespace Sentinel

namespace Scribe {
    namespace Rendering {
        class ScribeImGuiBase final: public Sentinel::Layer {
        public:
            ScribeImGuiBase(Sentinel::ContextData* context);
            ~ScribeImGuiBase();

        private:
            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnRender();
            void OnImGuiRender();
            void OnPostRender();

        private:
            Sentinel::PoolAllocator<Sentinel::Texture2DData> m_TexMemAllocator;

            Sentinel::ContextData* m_Context;
            Sentinel::Texture2DData* m_LogoTexture;
        };
    }  // namespace Rendering
}  // namespace Scribe