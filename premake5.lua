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
VendorIncludeDir["ENTT"] = "%{wks.location}/Snow/vendor/ENTT/include"
VendorIncludeDir["Glad"] = "%{wks.location}/Snow/vendor/Glad/include"
VendorIncludeDir["GLFW"] = "%{wks.location}/Snow/vendor/GLFW/include"
VendorIncludeDir["ImGui"] = "%{wks.location}/Snow/vendor/imgui"
VendorIncludeDir["stb"] = "%{wks.location}/Snow/vendor/stb"
VendorIncludeDir["Vulkan"] = "%{wks.location}/Snow/vendor/vulkan/Include"
VendorIncludeDir["shaderc"] = "%{wks.location}/Snow/vendor/shaderc/Include"
VendorIncludeDir["SPIRVCross"] = "%{wks.location}/Snow/vendor/SPIRV-Cross"


VendorLibraryDir = {}
VendorLibraryDir["Vulkan"] = "%{wks.location}/Snow/vendor/vulkan/Lib"
VendorLibraryDir["shaderc"] = "%{wks.location}/Snow/vendor/shaderc/Lib"
VendorLibraryDir["glslang"] = "%{wks.location}/Snow/vendor/glslang/Lib"
VendorLibraryDir["SPIRVTools"] = "%{wks.location}/Snow/vendor/SPIRV-Tools/Lib"
--VendorLibraryDir["LinuxVulkan"] = "%{wks.location}/Snow/vendor/vulkan/Lib/Linux"--/vulkan.so.1.2.148"
--VendorLibraryDir["Linuxshaderc"] = "%{wks.location}/Snow/vendor/shaderc/lib/Linux"--/shaderc.a"
--VendorLibraryDir["Linuxshaderc_util"] = "%{wks.location}/Snow/vendor/shaderc/lib/Linux"--/shaderc_util.a"


group "Vendor"
    include "Snow/vendor/Glad"
    include "Snow/vendor/GLFW"
    include "Snow/vendor/imgui"
    include "Snow/vendor/SPIRV-Cross"
group ""

include "Snow"
include "Glacier"