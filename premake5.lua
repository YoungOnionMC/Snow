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


project "Snow"
    location "Snow"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.*"
    }

    includedirs {
        "%{prj.name}/src",

    }
    
    links {
        
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

    end

    if os.target() == "linux" then
        defines {
            "SNOW_PLATFORM_LINUX"

        }

        links {
            "GL",
            "XLib",
            "X11",
            "Xi",

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

    links { "Snow" }

    files {
        "%{prj.name}/src/**.*"
    }

    includedirs {
        "%{prj.name}/src",
        "Snow/src"
    }