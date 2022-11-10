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

        "src/Snow/Asset/**.*",
        "src/Snow/Core/**.*",
        "src/Snow/Editor/**.*",
        "src/Snow/ImGui/**.*",
        "src/Snow/Physics/**.*",
        "src/Snow/Project/**.*",
        "src/Snow/Render/**.*",
        "src/Snow/Math/**.*",
        "src/Snow/Scene/**.*",
        "src/Snow/Script/**.*",
        "src/Snow/Utils/**.*",
        --"src/Snow/Platform/OpenGL/**.*",
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

        "vendor/ImGuizmo/ImGuizmo.cpp",
        "vendor/ImGuizmo/ImGuizmo.h",

        "vendor/VulkanMemoryAllocator/**.h",
        "vendor/VulkanMemoryAllocator/**.cpp",

        "vendor/ENTT/include/entt.hpp"
        
    }

    includedirs {
        "src",
        "vendor/spdlog/include",
        "%{VendorIncludeDir.Assimp}",
        "%{VendorIncludeDir.box2d}",
        "%{VendorIncludeDir.ENTT}",
        "%{VendorIncludeDir.freetype}",
        "%{VendorIncludeDir.Glad}",
        "%{VendorIncludeDir.GLFW}",
        "%{VendorIncludeDir.glm}",
        "%{VendorIncludeDir.ImGui}",
        "%{VendorIncludeDir.ImGuizmo}",
        "%{VendorIncludeDir.mono}",
        "%{VendorIncludeDir.stb}",
        "%{VendorIncludeDir.SPIRVCross}",
        "%{VendorIncludeDir.VMA}",
        "%{VendorIncludeDir.yaml_cpp}",
        
    }
    
    links {
        "Glad",
        "YAML",
        "Box2D",
    }

    defines {
        "GLFW_INCLUDE_NONE",
        "GLM_FORCE_SWIZZLE",

        "SNOW_ENABLE_ASSERTS",
    }

    flags { "NoPCH" }

    if os.target() == "windows" then
        defines { 
            "SNOW_PLATFORM_WINDOWS",
            --"SNOW_WINDOW_WIN32",
            "SNOW_WINDOW_GLFW",
        }

        characterset("MBCS")

        files {
            --"src/Snow/Platform/DirectX11/**.*",
            "src/Snow/Platform/Windows/**.*",
        }

        includedirs {
            "%{VendorIncludeDir.Vulkan}/Windows", 
            
            "%{VendorIncludeDir.shaderc}",
        }
        

        libdirs {
            "%{VendorLibraryDir.Assimp}/Windows",
            "%{VendorLibraryDir.Vulkan}/Windows",
            
            "%{VendorLibraryDir.glslang}/Windows",
            "%{VendorLibraryDir.SPIRVTools}/Windows",
            "%{VendorLibraryDir.mono}/Windows",
             
        }

        links {
            "opengl32.lib",
            "dxgi.lib",
            "dxguid.lib",
            "d3d11.lib",
            "d3d12.lib",
            "D3DCompiler.lib",

            "vulkan-1.lib",

            "GLFW",
            "ImGui",
            
            "SPIRVCross",

            "mono-2.0-sgen.lib",
            "eglib.lib",
        }

        filter "configurations:Debug"
            libdirs {
                "%{VendorLibraryDir.Assimp}/Windows/Debug",
                "%{VendorLibraryDir.glslang}/Windows/Debug",
                "%{VendorLibraryDir.SPIRVTools}/Windows/Debug",
                "%{VendorLibraryDir.shaderc}/Windows-Debug",
                "%{VendorLibraryDir.freetype}/Windows",
            }

            links {
                "glslangd.lib",
                "MachineIndependentd.lib",
                "SPIRVd.lib",
                "OGLCompilerd.lib",
                "OSDependentd.lib",
                "GenericCodeGend.lib",

                
                "shaderc.lib",
                "shaderc_util.lib",

                "SPIRV-Tools.lib",
                "SPIRV-Tools-opt.lib",
                
                "assimp-vc142-mtd.lib",

                "freetyped.lib",
            }

        filter "configurations:Release"
            libdirs {
                "%{VendorLibraryDir.Assimp}/Windows/Release",
                "%{VendorLibraryDir.glslang}/Windows/Release",
                "%{VendorLibraryDir.SPIRVTools}/Windows/Release",

                "%{VendorLibraryDir.shaderc}/Windows-Release",
            }

            links {
                "glslang.lib",
                "MachineIndependent.lib",
                "SPIRV.lib",
                "OGLCompiler.lib",
                "OSDependent.lib",
                "GenericCodeGen.lib",

                "shaderc.lib",
                "shaderc_util.lib",

                "SPIRV-Tools.lib",
                "SPIRV-Tools-opt.lib",

                "assimp-vc142-mt.lib",
            }

        filter ""

        

        

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
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "SNOW_RELEASE" }
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines { "SNOW_DIST" }
        runtime "Release"
        optimize "On"



    filter ""