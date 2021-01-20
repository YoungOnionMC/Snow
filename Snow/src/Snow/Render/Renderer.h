#pragma once
#include "Snow/Render/RenderContext.h"
#include "Snow/Render/RenderCommand.h"
#include "Snow/Render/Material.h"
#include "Snow/Render/Pipeline.h"
#include "Snow/Render/Shader/ShaderLibrary.h"

#include "Snow/Core/Event/Event.h"
#include "Snow/Core/Window.h"

namespace Snow {
    namespace Render {
        
        class Renderer {
        public:
            static void Init();

            static void BeginRenderPass(Ref<RenderPass> renderPass, bool clear = true);
            static void EndRenderPass();

            static void SubmitFullscreenQuad(Ref<MaterialInstance> material);

            //static void SwapBuffers();

            static void SetRenderAPI(RenderAPIType api) { s_RenderAPI = api; }
            static RenderAPIType GetRenderAPI() { return s_RenderAPI; }

            static Renderer* Get() { return s_Instance; }
            static Context* GetContext() {return s_Context; }

            static Ref<ShaderLibrary> GetShaderLibrary();

        private:
            static Renderer* s_Instance;

            static RenderAPIType s_RenderAPI;
            static Context* s_Context;
        };
    }
}