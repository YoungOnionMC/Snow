#include "Snow/Core/Application.h"
#include "Snow/EntryPoint.h"

#include "EditorLayer.h"
#include "RayTracing/RayTracingLayer.h"
#include "VulkanLayer.h"

namespace Snow {
    class MainApplication : public Core::Application {
    public:
        MainApplication() {
            //PushLayer(new RayTracingLayer());
            PushLayer(new EditorLayer());
            //PushLayer(new VulkanLayer());
        }
    };

    Core::Application* Core::CreateApplication() {
        return new MainApplication();
    }
}
