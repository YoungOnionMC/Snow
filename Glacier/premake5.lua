project "Glacier"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    

    files {
        "src/**.*"
    }

    includedirs {
        "src",
        "%{wks.location}/Snow/src",
        "%{wks.location}/Snow/vendor/spdlog/include",
        "%{VendorIncludeDir.ImGui}",

        --"%{VendorIncludeDir.GLFW}"
    }

    if os.target() == "windows" then
        links {
            "Snow"
        }
    end

    if os.target() == "linux" then
        
        --libdirs {
        --    "%{LibraryIncludeDir.LinuxVulkan}",
        --    "%{LibraryIncludeDir.shaderc}/Linux",
--
        --}

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
            "Snow",
            "Glad",
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
            "SPIRV",
            "OGLCompiler",
            "OSDependent",
            "GenericCodeGen",

            "SPIRV-Tools-opt",
            "SPIRV-Tools",

            "SPIRVCross",
        }
    end


    