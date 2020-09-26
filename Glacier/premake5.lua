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

            "%{LibraryIncludeDir.LinuxVulkan}",
            "%{LibraryIncludeDir.Linuxshaderc}",
            "%{LibraryIncludeDir.Linuxshaderc_util}",
        }
    end


    