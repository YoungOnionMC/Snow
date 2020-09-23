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
VendorIncludeDir["WindowsVulkan"] = "%{wks.location}/Snow/vendor/vulkan/Include"

LibraryIncludeDir = {}
LibraryIncludeDir["WindowsVulkan"] = "%{wks.location}/Snow/vendor/vulkan/Lib/Windows/vulkan-1.lib"
LibraryIncludeDir["LinuxVulkan"] = "%{wks.location}/Snow/vendor/vulkan/Lib/Linux/libvulkan.so"


group "Vendor"
    include "Snow/vendor/Glad"
    include "Snow/vendor/GLFW"
    include "Snow/vendor/imgui"
group ""

include "Snow"
include "Glacier"