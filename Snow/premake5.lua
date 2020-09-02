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
        "src/Snow/Platform/OpenGL/**.*"
    }

    includedirs {
        "src",
        "vendor/spdlog/include",
        "%{VendorIncludeDir.GLFW}",
        "%{VendorIncludeDir.Glad}",
    }
    
    links {
        "GLFW",
        "Glad"
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
            "src/Snow/Platform/Windows/**.*"
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
            "src/Snow/Platform/Linux/**.*" 
        }

    end

    filter "configurations:Debug"
        defines { "SNOW_DEBUG" }



    filter ""