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

    links { 
        "Snow",
        "GLFW",
        "Glad",
        "X11",
        "Xi",
        "pthread",
        "dl",
    }