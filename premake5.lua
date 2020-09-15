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
VendorIncludeDir["Vulkan"] = "%{wks.location}/Snow/vendor/vulkan/include"

LibraryIncludeDir = {}
LibraryIncludeDir["VulkanLinux"] = "vendor/vulkan/lib/libvulkan.so"


group "Vendor"
    include "Snow/vendor/Glad"
    include "Snow/vendor/GLFW"
group ""

include "Snow"
include "Glacier"