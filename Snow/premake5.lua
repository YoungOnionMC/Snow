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
        "src/Snow/Platform/Vulkan/**.*",

        
        "vendor/stb/stb_image.cpp",
        "vendor/stb/stb_image.h",
        "vendor/stb/stb_image_resize.h",
        "vendor/stb/stb_image_write.h",
        "vendor/stb/stb_include.h",
        "vendor/stb/stb_perlin.h",
        "vendor/stb/stb_rect_pack.h",
        "vendor/stb/stb_truetype.h",
        "vendor/stb/stb_textedit.h",
        "vendor/stb/stb_tilemap_editor.h",
        "vendor/stb/stb.h",
        
    }

    includedirs {
        "src",
        "vendor/spdlog/include",
        "%{VendorIncludeDir.GLFW}",
        "%{VendorIncludeDir.Glad}",
        "%{VendorIncludeDir.ImGui}",
        "%{VendorIncludeDir.stb}",
        "%{VendorIncludeDir.SPIRVCross}",
        
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
            "%{VendorIncludeDir.Vulkan}/Windows", 
            "%{VendorIncludeDir.shaderc}",
        }
        
        libdirs {
            "%{VendorLibraryDir.Vulkan}/Windows",
            "%{VendorLibraryDir.shaderc}/Windows",
            "%{VendorLibraryDir.glslang}/Windows",
            "%{VendorLibraryDir.SPIRVTools}/Windows",
        }

        links {
            "opengl32.lib",
            "dxgi.lib",
            "d3d11.lib",
            "d3d12.lib",
            "D3DCompiler.lib",

            "vulkan-1.lib",
            "shaderc.lib",
            "shaderc_util.lib",

            "glslangd.lib",
            "MachineIndependentd.lib",
            "SPIRVd.lib",
            "OGLCompilerd.lib",
            "OSDependentd.lib",
            "GenericCodeGend.lib",

            "SPIRV-Tools.lib",
            "SPIRV-Tools-opt.lib",
            

            "GLFW",
            "ImGui",

            "SPIRVCross",
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
            "%{VendorIncludeDir.Vulkan}/Linux",
            "%{VendorIncludeDir.shaderc}",
        }

        libdirs {
            "%{VendorLibraryDir.Vulkan}/Linux",
            "%{VendorLibraryDir.shaderc}/Linux",
            "%{VendorLibraryDir.glslang}/Linux",
            "%{VendorLibraryDir.SPIRVTools}/Linux",
        }

        links {
            --"GL",
            
            
            "ImGui",
            
            "GLFW",
            "X11",
            "Xi",
            "xcb",
            "pthread",
            "dl",

            "vulkan",
            
            --"glslc",
            "shaderc",
            "shaderc_util",
            "glslang",
            "MachineIndependent",
            "libSPIRV",
            "OGLCompiler",
            "OSDependent",
            "GenericCodeGen",

            "SPIRV-Tools-opt",
            "SPIRV-Tools",

            "SPIRVCross",

        }

        files { 
            "src/Snow/Platform/Linux/**.*" 
        }

    end

    filter "configurations:Debug"
        defines { "SNOW_DEBUG" }



    filter ""