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
        "src/spch.h",
        "src/spch.cpp",
        "src/Snow.h",
        "src/Snow/EntryPoint.h",

        "src/Snow/Core/**.*",
        "src/Snow/ImGui/**.*",
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
        "%{VendorIncludeDir.ImGui}",
        
    }
    
    links {
        "Glad",
    }

    defines {
        "GLFW_INCLUDE_NONE"
    }

    flags { "NoPCH" }

    if os.target() == "windows" then
        defines { 
            "SNOW_PLATFORM_WINDOWS",
            --"SNOW_WINDOW_WIN32",
            "SNOW_WINDOW_GLFW",
        }

        characterset("MBCS")

        includedirs {
            "%{VendorIncludeDir.WindowsVulkan}",  
		}

        links {
            "opengl32.lib",
            "dxgi.lib",
            "d3d11.lib",
            "d3d12.lib",
            "D3DCompiler.lib",

            "%{LibraryIncludeDir.WindowsVulkan}",

            "GLFW",
            "ImGui"
        }

        files {
            "src/Snow/Platform/Windows/**.*"
        }

    end

    if os.target() == "linux" then
        defines {
            "SNOW_PLATFORM_LINUX",
            --"SNOW_WINDOW_XCB",
            --"SNOW_WINDOW_XLIB",
            "SNOW_WINDOW_GLFW",
        }

        includedirs {
            "%{VendorIncludeDir.LinuxVulkan}"
        }

        links {
            --"GL",
            --"%{LibraryIncludeDir.LinuxVulkan}",
            "vulkan",
            "ImGui",
            
            "GLFW",
            "X11",
            "Xi",
            "xcb",
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