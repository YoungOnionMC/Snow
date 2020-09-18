#include <spch.h>


#include "Snow/Render/Renderer.h"
#include "Snow/Core/Application.h"

namespace Snow {
    namespace Render {

        namespace Event {
            RenderResizeListener::RenderResizeListener() {
                SetEventType(Core::Event::WindowResizeEvent::ID);
            }

            void RenderResizeListener::HandleEvent(Core::Event::BaseEvent* event) {
                Core::Event::WindowResizeEvent* e = (Core::Event::WindowResizeEvent*)event;
                Renderer::SetViewport(e->GetWidth(), e->GetHeight());
            }
        }

        Renderer* Renderer::s_Instance = nullptr;
        RenderAPI Renderer::s_RenderAPI = RenderAPI::None;
        RenderCommand* Renderer::s_RenderCommand = nullptr;
        Context* Renderer::s_Context = nullptr;

        Event::RenderResizeListener* m_RenderResizeListener = nullptr;

        void Renderer::Init() {
            SNOW_CORE_INFO("Initializing Renderer");
            ContextSpecification contextSpec;
            contextSpec.s_RenderAPI = RenderAPI::OpenGL;
            contextSpec.WindowHandle = Core::Application::Get().GetWindow()->GetWindowHandle();

            s_RenderAPI = contextSpec.s_RenderAPI;
            s_Context = Context::Create(contextSpec);

            m_RenderResizeListener = new Event::RenderResizeListener();
            Core::Event::EventSystem::AddListener(m_RenderResizeListener);

            s_RenderCommand = RenderCommand::Create();
        }

        void Renderer::BeginScene() {
            s_RenderCommand->BeginScene();
        }

        void Renderer::EndScene() {
            s_RenderCommand->EndScene();
        }

        void Renderer::DrawIndexed(uint32_t count) {
            s_RenderCommand->DrawIndexed(count);
        }

        void Renderer::SetViewport(int width, int height) {
            s_RenderCommand->SetViewport(width, height);
        }
    }
}