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
        "%{VendorIncludeDir.ENTT}",
        "%{VendorIncludeDir.ImGui}",
        --"%{VendorIncludeDir.GLFW}"
    }

    postbuildcommands {
        '{COPY} "../Glacier/assets" "%{cfg.targetdir}/assets"'
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
            "%{wks.location}/Snow/vendor/ENTT/include",
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


    