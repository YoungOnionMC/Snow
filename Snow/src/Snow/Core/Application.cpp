#include <spch.h>

#include "Snow/Core/Application.h"
#include "Snow/Render/Renderer.h"

#include "Snow/Render/Renderer2D.h"

namespace Snow {
    namespace Core {


        Application* Application::s_Instance = nullptr;

       

        Application::Application() {
            SNOW_CORE_INFO("Creating Application...");
            s_Instance = this;

            Render::Renderer::SetRenderAPI(Render::RenderAPIType::DirectX);
            m_Window = new Window();
            m_Window->SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent));
            Input::Init();
            Input::SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent));

            m_LayerStack = LayerStack();

            Render::Renderer::Init();
            m_ImGuiLayer = ImGuiLayer::Create();
            m_LayerStack.PushOverlay(m_ImGuiLayer);
        }

        Application::~Application() {
            //delete m_Window;
            SNOW_CORE_TRACE("Destroying Application");
        }

        void Application::OnImGuiRender() {
            m_ImGuiLayer->BeginImGuiFrame();

            for(Layer* layer : m_LayerStack)
                layer->OnImGuiRender();

            m_ImGuiLayer->EndImGuiFrame();
        }

        void Application::Run() {
            while(m_Running) {


                
                //Render::Renderer::BeginScene();
                OnUpdate();
                

                OnImGuiRender();

                //Render::Renderer::SwapBuffers();
                //Render::Renderer::EndScene();

            }
        }

        void Application::OnUpdate() {
            m_Window->OnUpdate();

            for(Layer* layer : m_LayerStack)
                layer->OnUpdate();
        }

        void Application::OnEvent(Event::Event& e) {
            Event::EventDispatcher dispatcher(e);
            dispatcher.Dispatch<Event::WindowResizeEvent>(SNOW_BIND_EVENT_FN(Application::OnApplicationResize));
            dispatcher.Dispatch<Event::WindowCloseEvent>(SNOW_BIND_EVENT_FN(Application::OnApplicationClose));
            dispatcher.Dispatch<Event::AppUpdateEvent>(SNOW_BIND_EVENT_FN(Application::OnApplicationUpdate));

            for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
                (*--it)->OnEvent(e);
                if (e.Handled)
                    break;
            }
        }

        bool Application::OnApplicationUpdate(Event::AppUpdateEvent& e) {
            return false;
        }

        bool Application::OnApplicationResize(Event::WindowResizeEvent& e) {
            Render::RenderCommand::SetViewport(e.GetWidth(), e.GetHeight());

            return false;
        }

        bool Application::OnApplicationClose(Event::WindowCloseEvent& e) {
            m_Running = false;
            return true;
        }
    }
}