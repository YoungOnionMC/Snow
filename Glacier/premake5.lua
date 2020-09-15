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

        "%{VendorIncludeDir.GLFW}"
    }

    if os.target() == "windows" then
        links {
            "Snow"
        }
    end

    if os.target() == "linux" then
    links { 
        "Snow",
        "vulkan",
        "GLFW",
        "Glad",
        "X11",
        "Xi",
        "xcb",
        "pthread",
        "dl",
    }
    end


    