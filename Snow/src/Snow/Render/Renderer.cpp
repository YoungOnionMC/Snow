#include <spch.h>


#include "Snow/Render/Renderer.h"
#include "Snow/Core/Application.h"

#include "Snow/Render/Renderer2D.h"

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
            contextSpec.s_RenderAPI = GetRenderAPI();
            contextSpec.WindowHandle = Core::Application::Get().GetWindow()->GetWindowHandle();

            
            s_Context = Context::Create(contextSpec);

            m_RenderResizeListener = new Event::RenderResizeListener();
            Core::Event::EventSystem::AddListener(m_RenderResizeListener);

            s_RenderCommand = RenderCommand::Create();

            Renderer2D::Init();
        }

        void Renderer::BeginScene() {
            s_RenderCommand->BeginScene();
        }

        void Renderer::EndScene() {
            s_RenderCommand->EndScene();
        }

        void Renderer::DrawIndexed(uint32_t count, PrimitiveType type) {
            s_RenderCommand->DrawIndexed(count, type);
        }

        void Renderer::SetViewport(int width, int height) {
            s_RenderCommand->SetViewport(width, height);
        }

        void Renderer::ClearMainColorAttachment(const Math::Vector4f& color) {
            s_RenderCommand->ClearColorAttachment(color);
        }
    }
}