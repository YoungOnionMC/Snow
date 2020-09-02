workspace "Snow"
    architecture "x86_64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }
    
    startproject "Glacier"





outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VendorIncludeDir = {}
VendorIncludeDir["GLFW"] = "Snow/vendor/GLFW/include"




include "Snow/vendor/GLFW"


project "Snow"
    location "Snow"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "%{prj.name}/src/spch.h"
    pchsource "%{prj.name}/src/spch.cpp"


    files {
        "%{prj.name}/src/Snow/Core/**.*"
    }

    includedirs {
        "%{prj.name}/src",
        "%{VendorIncludeDir.GLFW}",

        "%{prj.name}/vendor/spdlog/include"
    }
    
    links {
        "GLFW"
    }

    defines {
        "GLFW_INCLUDE_NONE"
    }

    if os.target() == "windows" then
        defines { "SNOW_PLATFORM_WINDOWS" }

        characterset("MBCS")

        links {
            "opengl32.lib",
            "dxgi.lib",
            "d3d11.lib",
            "d3d12.lib",
            "D3DCompiler.lib"
        }

        files {
            "%{prj.name}/src/Snow/Platform/Windows/**.*"
        }

    end

    if os.target() == "linux" then
        defines {
            "SNOW_PLATFORM_LINUX"

        }

        links {
            --"GL",
            "X11",
            "Xi",
            "pthread",
            "dl",

        }

        files { 
            "%{prj.name}/src/Snow/Platform/Linux/**.*" 
        }

    end

    filter "configurations:Debug"
        defines { "SNOW_DEBUG" }



    filter ""




project "Glacier"
    location "Glacier"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    links { 
        "Snow",
        "GLFW",
        "X11",
        "Xi",
        "pthread",
        "dl"
    }

    files {
        "%{prj.name}/src/**.*"
    }

    includedirs {
        "%{prj.name}/src",
        "Snow/src",
        "Snow/vendor/spdlog/include",
    }