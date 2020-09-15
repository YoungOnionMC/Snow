project "Snow"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "spch.h"
    pchsource "src/spch.cpp"


    files {
        "src/Snow/Core/**.*",
        "src/Snow/Render/**.*",
        "src/Snow/Math/**.*",
        "src/Snow/Platform/OpenGL/**.*",
        "src/Snow/Platform/Vulkan/**.*"
    }

    includedirs {
        "src",
        "vendor/spdlog/include",
        "%{VendorIncludeDir.GLFW}",
        "%{VendorIncludeDir.Glad}",
        "%{VendorIncludeDir.Vulkan}"
    }
    
    links {
        "Glad",
        "GLFW",
    }

    defines {
        "GLFW_INCLUDE_NONE"
    }

    flags { "NoPCH" }

    if os.target() == "windows" then
        defines { 
            "SNOW_PLATFORM_WINDOWS",
            "SNOW_WINDOW_WIN32",
            --"SNOW_WINDOW_GLFW"
        }

        characterset("MBCS")

        links {
            "opengl32.lib",
            "dxgi.lib",
            "d3d11.lib",
            "d3d12.lib",
            "D3DCompiler.lib"
        }

        files {
            "src/Snow/Platform/Windows/**.*"
        }

    end

    if os.target() == "linux" then
        defines {
            "SNOW_PLATFORM_LINUX",
            "VK_USE_PLATFORM_XLIB_KHR",
            "GLFW_VULKAN_STATIC"
        }

        links {
            --"GL",
            "vulkan",
            
            "X11",
            "Xi",
            "pthread",
            "dl",
        }

        files { 
            "src/Snow/Platform/Linux/**.*" 
        }

    end

    filter "configurations:Debug"
        defines { "SNOW_DEBUG" }



    filter ""