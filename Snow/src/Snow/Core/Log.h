#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Snow {
    namespace Core {
        class Logger {
        public:
            static void Init();

            inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
            inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        private:
            static std::shared_ptr<spdlog::logger> s_CoreLogger;
            static std::shared_ptr<spdlog::logger> s_ClientLogger;

        };
    }
}

#define SNOW_CORE_TRACE(...)    ::Snow::Core::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define SNOW_CORE_INFO(...)    ::Snow::Core::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define SNOW_CORE_WARN(...)    ::Snow::Core::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define SNOW_CORE_ERROR(...)    ::Snow::Core::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define SNOW_CORE_CRITICAL(...)    ::Snow::Core::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define SNOW_CLIENT_TRACE(...)    ::Snow::Core::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define SNOW_CLIENT_INFO(...)    ::Snow::Core::Logger::GetClientLogger()->info(__VA_ARGS__)
#define SNOW_CLIENT_WARN(...)    ::Snow::Core::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define SNOW_CLIENT_ERROR(...)    ::Snow::Core::Logger::GetClientLogger()->error(__VA_ARGS__)
#define SNOW_CLIENT_CRITICAL(...)    ::Snow::Core::Logger::GetClientLogger()->critical(__VA_ARGS__)