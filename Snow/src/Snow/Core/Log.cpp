#include <spch.h>

#include "Snow/Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/bundled/color.h>

namespace Snow {
    namespace Core {
        std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
        std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

        void Logger::Init() {
            s_CoreLogger = spdlog::stdout_color_mt("Core");
            s_CoreLogger->set_pattern("(%T) %n - %^%v%$ ");
            s_CoreLogger->set_level(spdlog::level::trace);

            auto coreSink = static_cast<spdlog::sinks::stdout_color_sink_mt*>(s_CoreLogger->sinks()[0].get());
            coreSink->set_color(spdlog::level::trace, coreSink->CYAN);
            //coreSink->set_color(spdlog::level::trace, coreSink->cyan);
            coreSink->set_color(spdlog::level::critical, coreSink->RED);
            //coreSink->set_color(spdlog::level::critical, coreSink->red);

            s_ClientLogger = spdlog::stdout_color_mt("App");
            s_ClientLogger->set_pattern("(%T) %n - %^%v%$ ");
            s_ClientLogger->set_level(spdlog::level::trace);

            auto clientSink = static_cast<spdlog::sinks::stdout_color_sink_mt*>(s_ClientLogger->sinks()[0].get());
            //clientSink->set_color(spdlog::level::trace, clientSink->cyan);
            clientSink->set_color(spdlog::level::trace, clientSink->CYAN);
            //clientSink->set_color(spdlog::level::critical, clientSink->red);
            clientSink->set_color(spdlog::level::critical, clientSink->RED);


        }
    }
}