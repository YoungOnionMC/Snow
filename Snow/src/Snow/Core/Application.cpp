#include <spch.h>

#include "Snow/Core/Application.h"
#include "Snow/Render/Renderer.h"

#include "Snow/Render/Renderer2D.h"

#include "Snow/Script/ScriptEngine.h"

namespace Snow {
    namespace Core {


        Application* Application::s_Instance = nullptr;

       

        Application::Application() {
            SNOW_CORE_INFO("Creating Application...");
            s_Instance = this;

            //Render::Renderer(Render::RendererAPIType::Vulkan);
            m_Window = new Window();
            if (!m_Window->PlatformInit()) {
                SNOW_CORE_ERROR("Window creation failed");
            }
            SNOW_CORE_INFO("Window Created");

            m_Window->SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent));
            Input::Init();
            Input::SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent));

            m_LayerStack = LayerStack();

            Render::Renderer::Init();
            m_ImGuiLayer = ImGuiLayer::Create();
            m_LayerStack.PushOverlay(m_ImGuiLayer);


            Script::ScriptEngine::Init("assets/scripts/ExampleScriptApp.dll");
        }

        Application::~Application() {
            //delete m_Window;
            SNOW_CORE_TRACE("Destroying Application");
        }

        void Application::OnImGuiRender() {
            m_ImGuiLayer->BeginImGuiFrame();
            ImGui::Begin("RendererStats");
            ImGui::Text("Frame Time: %.2fms\n", m_Timestep.GetMilliseconds());

            ImGui::End();

            for(Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
        }

        void Application::Run() {
            while(m_Running) {

                m_Window->ProcessEvents();
                Render::Renderer::BeginFrame();
               

                OnUpdate(m_Timestep);
                
                Application* app = this;
                Render::Renderer::Submit([app]() {app->OnImGuiRender(); });
                Render::Renderer::Submit([=]() {m_ImGuiLayer->EndImGuiFrame(); });
                Render::Renderer::EndFrame();
                // End Frame

                m_Window->GetSwapChain()->BeginFrame();
                Render::Renderer::WaitAndRender();
                m_Window->SwapBuffers();

                //Render::Renderer::SwapBuffers();
                //Render::Renderer::EndScene();

                float time = m_Window->GetSystemTime();
                m_Timestep = time - m_LastFrameTime;
                m_LastFrameTime = time;
            }
        }

        void Application::OnUpdate(Timestep ts) {
            for(Layer* layer : m_LayerStack)
                layer->OnUpdate(ts);

            
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
            const uint32_t width = e.GetWidth(), height = e.GetHeight();
            //Render::RenderCommand::SetViewport(e.GetWidth(), e.GetHeight());

            m_Window->GetSwapChain()->OnResize(width, height);

            return false;
        }

        bool Application::OnApplicationClose(Event::WindowCloseEvent& e) {
            m_Running = false;
            return true;
        }
    }
}