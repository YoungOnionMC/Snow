workspace "Snow"
    architecture "x86_64"
    startproject "Glacier"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }
    





outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VendorIncludeDir = {}
VendorIncludeDir["Assimp"] = "%{wks.location}/Snow/vendor/assimp/include"
VendorIncludeDir["box2d"] = "%{wks.location}/Snow/vendor/box2d/include"
VendorIncludeDir["ENTT"] = "%{wks.location}/Snow/vendor/ENTT/include"
VendorIncludeDir["freetype"] = "%{wks.location}/Snow/vendor/freetype/include"
VendorIncludeDir["Glad"] = "%{wks.location}/Snow/vendor/Glad/include"
VendorIncludeDir["GLFW"] = "%{wks.location}/Snow/vendor/GLFW/include"
VendorIncludeDir["glm"] = "%{wks.location}/Snow/vendor/glm"
VendorIncludeDir["ImGui"] = "%{wks.location}/Snow/vendor/imgui"
VendorIncludeDir["ImGuizmo"] = "%{wks.location}/Snow/vendor/ImGuizmo"
VendorIncludeDir["mono"] = "%{wks.location}/Snow/vendor/mono/include"
VendorIncludeDir["stb"] = "%{wks.location}/Snow/vendor/stb"
VendorIncludeDir["shaderc"] = "%{wks.location}/Snow/vendor/shaderc/Include"
VendorIncludeDir["SPIRVCross"] = "%{wks.location}/Snow/vendor/SPIRV-Cross"
VendorIncludeDir["Vulkan"] = "%{wks.location}/Snow/vendor/vulkan/Include"
VendorIncludeDir["VMA"] = "%{wks.location}/Snow/vendor/vulkanMemoryAllocator"
VendorIncludeDir["yaml_cpp"] = "%{wks.location}/Snow/vendor/yaml-cpp/include"


VendorLibraryDir = {}
VendorLibraryDir["Assimp"] = "%{wks.location}/Snow/vendor/assimp/Lib"
VendorLibraryDir["Vulkan"] = "%{wks.location}/Snow/vendor/vulkan/Lib"
VendorLibraryDir["shaderc"] = "%{wks.location}/Snow/vendor/shaderc/Lib"
VendorLibraryDir["glslang"] = "%{wks.location}/Snow/vendor/glslang/Lib"
VendorLibraryDir["SPIRVTools"] = "%{wks.location}/Snow/vendor/SPIRV-Tools/Lib"
VendorLibraryDir["mono"] = "%{wks.location}/Snow/vendor/mono/lib"
VendorLibraryDir["freetype"] = "%{wks.location}/Snow/vendor/freetype/Lib"
--VendorLibraryDir["LinuxVulkan"] = "%{wks.location}/Snow/vendor/vulkan/Lib/Linux"--/vulkan.so.1.2.148"
--VendorLibraryDir["Linuxshaderc"] = "%{wks.location}/Snow/vendor/shaderc/lib/Linux"--/shaderc.a"
--VendorLibraryDir["Linuxshaderc_util"] = "%{wks.location}/Snow/vendor/shaderc/lib/Linux"--/shaderc_util.a"


group "Vendor"
    include "Snow/vendor/box2d" 
    include "Snow/vendor/Glad"
    include "Snow/vendor/GLFW"
    include "Snow/vendor/imgui"
    include "Snow/vendor/SPIRV-Cross"
    include "Snow/vendor/yaml-cpp"
group ""

group "Engine"
    include "Snow"
    include "SnowScriptCore"
group ""

group "Editor"
    include "Glacier"
group ""

group "Scripts"
    include "ExampleScriptApp"
group ""

workspace "Script"
    architecture "x64"
    targetdir "build"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"   

group "Scripting"
    include "SnowScriptCore"
    include "ExampleScriptApp"
group ""