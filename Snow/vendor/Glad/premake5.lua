project "Glad"
    kind "StaticLib"
    language "C++"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "include"
    }

    files {
        "include/glad/glad.h",
        "src/glad.c"
    }

    if os.target() == "windows" then
        files {
            "include/glad/glad_wgl.h",
            "src/glad_wgl.c"
        }
    end

    if os.target() == "linux" then
        files {
            "include/glad/glad_glx.h",
            "src/glad_glx.c"
        }
    end