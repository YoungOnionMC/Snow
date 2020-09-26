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
VendorIncludeDir["Glad"] = "%{wks.location}/Snow/vendor/Glad/include"
VendorIncludeDir["GLFW"] = "%{wks.location}/Snow/vendor/GLFW/include"
VendorIncludeDir["ImGui"] = "%{wks.location}/Snow/vendor/imgui"

VendorIncludeDir["WindowsVulkan"] = "%{wks.location}/Snow/vendor/vulkan/include/Windows"
VendorIncludeDir["LinuxVulkan"] = "%{wks.location}/Snow/vendor/vulkan/include/Linux"

VendorIncludeDir["LinuxShaderc"] = "%{wks.location}/Snow/vendor/shaderc/include/Linux"

LibraryIncludeDir = {}
LibraryIncludeDir["LinuxVulkan"] = "%{wks.location}/Snow/vendor/vulkan/Lib/Linux"--/vulkan.so.1.2.148"
LibraryIncludeDir["Linuxshaderc"] = "%{wks.location}/Snow/vendor/shaderc/lib/Linux"--/shaderc.a"
LibraryIncludeDir["Linuxshaderc_util"] = "%{wks.location}/Snow/vendor/shaderc/lib/Linux"--/shaderc_util.a"


group "Vendor"
    include "Snow/vendor/Glad"
    include "Snow/vendor/GLFW"
    include "Snow/vendor/imgui"
group ""

include "Snow"
include "Glacier"