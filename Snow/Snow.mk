##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Snow
ConfigurationName      :=Debug
WorkspacePath          :=/media/james/Dev/Snow
ProjectPath            :=/media/james/Dev/Snow/Snow
IntermediateDirectory  :=../bin-int/Debug-linux-x86_64/Snow
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=James
Date                   :=01/10/20
CodeLitePath           :=/home/james/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=../bin/Debug-linux-x86_64/Snow/libSnow.a
Preprocessors          :=$(PreprocessorSwitch)GLFW_INCLUDE_NONE $(PreprocessorSwitch)SNOW_PLATFORM_LINUX $(PreprocessorSwitch)SNOW_WINDOW_GLFW $(PreprocessorSwitch)SNOW_DEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Snow.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -m64 vendor/Glad/bin/Debug-linux-x86_64/Glad/libGlad.a vendor/imgui/bin/Debug-linux-x86_64/ImGui/libImGui.a vendor/GLFW/bin/Debug-linux-x86_64/GLFW/libGLFW.a vendor/SPIRV-Cross/bin/Debug-linux-x86_64/SPIRVCross/libSPIRVCross.a -lX11 -lXi -lxcb -lpthread -ldl -lvulkan -lshaderc -lshaderc_util -lglslang -lMachineIndependent -llibSPIRV -lOGLCompiler -lOSDependent -lGenericCodeGen -lSPIRV-Tools-opt -lSPIRV-Tools
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)src $(IncludeSwitch)vendor/spdlog/include $(IncludeSwitch)vendor/GLFW/include $(IncludeSwitch)vendor/Glad/include $(IncludeSwitch)vendor/imgui $(IncludeSwitch)vendor/stb $(IncludeSwitch)vendor/SPIRV-Cross $(IncludeSwitch)vendor/vulkan/Include/Linux $(IncludeSwitch)vendor/shaderc/Include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)vendor/vulkan/Lib/Linux $(LibraryPathSwitch)vendor/shaderc/Lib/Linux $(LibraryPathSwitch)vendor/glslang/Lib/Linux $(LibraryPathSwitch)vendor/SPIRV-Tools/Lib/Linux 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -m64 -std=c++17 $(Preprocessors)
CFLAGS   :=  -m64 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_Snow_Render_Shader_Shader.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_API_Texture.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_API_Framebuffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_API_Buffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanSwapChain.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanShader.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Core_Event_Event.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_SwapChain.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_Renderer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_RenderContext.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLShader.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_RenderCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanAllocator.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_Renderer2D.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Core_Event_EventSystem.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_spch.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Core_Window.cpp$(ObjectSuffix) $(IntermediateDirectory)/vendor_stb_stb_image.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLContext.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Core_Input.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_Snow_ImGui_ImGuiLayer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLRenderCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Core_Log.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLBuffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_Pipeline.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Math_Math.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Core_Layer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLTexture.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanDevice.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Core_Application.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanPipeline.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxInput.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLImGuiLayer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLPipeline.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLSwapChain.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Render_RenderPass.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanContext.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanImGuiLayer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanRenderCommand.cpp$(ObjectSuffix) \
	



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList)
	@$(MakeDirCommand) "/media/james/Dev/Snow/.build-debug"
	@echo rebuilt > "/media/james/Dev/Snow/.build-debug/Snow"

MakeIntermediateDirs:
	@test -d ../bin-int/Debug-linux-x86_64/Snow || $(MakeDirCommand) ../bin-int/Debug-linux-x86_64/Snow


../bin-int/Debug-linux-x86_64/Snow:
	@test -d ../bin-int/Debug-linux-x86_64/Snow || $(MakeDirCommand) ../bin-int/Debug-linux-x86_64/Snow

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_Snow_Render_Shader_Shader.cpp$(ObjectSuffix): src/Snow/Render/Shader/Shader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_Shader_Shader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_Shader_Shader.cpp$(DependSuffix) -MM src/Snow/Render/Shader/Shader.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/Shader/Shader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_Shader_Shader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_Shader_Shader.cpp$(PreprocessSuffix): src/Snow/Render/Shader/Shader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_Shader_Shader.cpp$(PreprocessSuffix) src/Snow/Render/Shader/Shader.cpp

$(IntermediateDirectory)/src_Snow_Render_API_Texture.cpp$(ObjectSuffix): src/Snow/Render/API/Texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_API_Texture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_API_Texture.cpp$(DependSuffix) -MM src/Snow/Render/API/Texture.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/API/Texture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_API_Texture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_API_Texture.cpp$(PreprocessSuffix): src/Snow/Render/API/Texture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_API_Texture.cpp$(PreprocessSuffix) src/Snow/Render/API/Texture.cpp

$(IntermediateDirectory)/src_Snow_Render_API_Framebuffer.cpp$(ObjectSuffix): src/Snow/Render/API/Framebuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_API_Framebuffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_API_Framebuffer.cpp$(DependSuffix) -MM src/Snow/Render/API/Framebuffer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/API/Framebuffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_API_Framebuffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_API_Framebuffer.cpp$(PreprocessSuffix): src/Snow/Render/API/Framebuffer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_API_Framebuffer.cpp$(PreprocessSuffix) src/Snow/Render/API/Framebuffer.cpp

$(IntermediateDirectory)/src_Snow_Render_API_Buffer.cpp$(ObjectSuffix): src/Snow/Render/API/Buffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_API_Buffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_API_Buffer.cpp$(DependSuffix) -MM src/Snow/Render/API/Buffer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/API/Buffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_API_Buffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_API_Buffer.cpp$(PreprocessSuffix): src/Snow/Render/API/Buffer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_API_Buffer.cpp$(PreprocessSuffix) src/Snow/Render/API/Buffer.cpp

$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanSwapChain.cpp$(ObjectSuffix): src/Snow/Platform/Vulkan/VulkanSwapChain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanSwapChain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanSwapChain.cpp$(DependSuffix) -MM src/Snow/Platform/Vulkan/VulkanSwapChain.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Vulkan/VulkanSwapChain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanSwapChain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanSwapChain.cpp$(PreprocessSuffix): src/Snow/Platform/Vulkan/VulkanSwapChain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanSwapChain.cpp$(PreprocessSuffix) src/Snow/Platform/Vulkan/VulkanSwapChain.cpp

$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanShader.cpp$(ObjectSuffix): src/Snow/Platform/Vulkan/VulkanShader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanShader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanShader.cpp$(DependSuffix) -MM src/Snow/Platform/Vulkan/VulkanShader.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Vulkan/VulkanShader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanShader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanShader.cpp$(PreprocessSuffix): src/Snow/Platform/Vulkan/VulkanShader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanShader.cpp$(PreprocessSuffix) src/Snow/Platform/Vulkan/VulkanShader.cpp

$(IntermediateDirectory)/src_Snow_Core_Event_Event.cpp$(ObjectSuffix): src/Snow/Core/Event/Event.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Core_Event_Event.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Core_Event_Event.cpp$(DependSuffix) -MM src/Snow/Core/Event/Event.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Core/Event/Event.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Core_Event_Event.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Core_Event_Event.cpp$(PreprocessSuffix): src/Snow/Core/Event/Event.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Core_Event_Event.cpp$(PreprocessSuffix) src/Snow/Core/Event/Event.cpp

$(IntermediateDirectory)/src_Snow_Render_SwapChain.cpp$(ObjectSuffix): src/Snow/Render/SwapChain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_SwapChain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_SwapChain.cpp$(DependSuffix) -MM src/Snow/Render/SwapChain.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/SwapChain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_SwapChain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_SwapChain.cpp$(PreprocessSuffix): src/Snow/Render/SwapChain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_SwapChain.cpp$(PreprocessSuffix) src/Snow/Render/SwapChain.cpp

$(IntermediateDirectory)/src_Snow_Render_Renderer.cpp$(ObjectSuffix): src/Snow/Render/Renderer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_Renderer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_Renderer.cpp$(DependSuffix) -MM src/Snow/Render/Renderer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/Renderer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_Renderer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_Renderer.cpp$(PreprocessSuffix): src/Snow/Render/Renderer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_Renderer.cpp$(PreprocessSuffix) src/Snow/Render/Renderer.cpp

$(IntermediateDirectory)/src_Snow_Render_RenderContext.cpp$(ObjectSuffix): src/Snow/Render/RenderContext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_RenderContext.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_RenderContext.cpp$(DependSuffix) -MM src/Snow/Render/RenderContext.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/RenderContext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_RenderContext.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_RenderContext.cpp$(PreprocessSuffix): src/Snow/Render/RenderContext.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_RenderContext.cpp$(PreprocessSuffix) src/Snow/Render/RenderContext.cpp

$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLShader.cpp$(ObjectSuffix): src/Snow/Platform/OpenGL/OpenGLShader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLShader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLShader.cpp$(DependSuffix) -MM src/Snow/Platform/OpenGL/OpenGLShader.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/OpenGL/OpenGLShader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLShader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLShader.cpp$(PreprocessSuffix): src/Snow/Platform/OpenGL/OpenGLShader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLShader.cpp$(PreprocessSuffix) src/Snow/Platform/OpenGL/OpenGLShader.cpp

$(IntermediateDirectory)/src_Snow_Render_RenderCommand.cpp$(ObjectSuffix): src/Snow/Render/RenderCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_RenderCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_RenderCommand.cpp$(DependSuffix) -MM src/Snow/Render/RenderCommand.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/RenderCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_RenderCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_RenderCommand.cpp$(PreprocessSuffix): src/Snow/Render/RenderCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_RenderCommand.cpp$(PreprocessSuffix) src/Snow/Render/RenderCommand.cpp

$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanAllocator.cpp$(ObjectSuffix): src/Snow/Platform/Vulkan/VulkanAllocator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanAllocator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanAllocator.cpp$(DependSuffix) -MM src/Snow/Platform/Vulkan/VulkanAllocator.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Vulkan/VulkanAllocator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanAllocator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanAllocator.cpp$(PreprocessSuffix): src/Snow/Platform/Vulkan/VulkanAllocator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanAllocator.cpp$(PreprocessSuffix) src/Snow/Platform/Vulkan/VulkanAllocator.cpp

$(IntermediateDirectory)/src_Snow_Render_Renderer2D.cpp$(ObjectSuffix): src/Snow/Render/Renderer2D.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_Renderer2D.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_Renderer2D.cpp$(DependSuffix) -MM src/Snow/Render/Renderer2D.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/Renderer2D.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_Renderer2D.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_Renderer2D.cpp$(PreprocessSuffix): src/Snow/Render/Renderer2D.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_Renderer2D.cpp$(PreprocessSuffix) src/Snow/Render/Renderer2D.cpp

$(IntermediateDirectory)/src_Snow_Core_Event_EventSystem.cpp$(ObjectSuffix): src/Snow/Core/Event/EventSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Core_Event_EventSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Core_Event_EventSystem.cpp$(DependSuffix) -MM src/Snow/Core/Event/EventSystem.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Core/Event/EventSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Core_Event_EventSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Core_Event_EventSystem.cpp$(PreprocessSuffix): src/Snow/Core/Event/EventSystem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Core_Event_EventSystem.cpp$(PreprocessSuffix) src/Snow/Core/Event/EventSystem.cpp

$(IntermediateDirectory)/src_spch.cpp$(ObjectSuffix): src/spch.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_spch.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_spch.cpp$(DependSuffix) -MM src/spch.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/spch.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_spch.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_spch.cpp$(PreprocessSuffix): src/spch.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_spch.cpp$(PreprocessSuffix) src/spch.cpp

$(IntermediateDirectory)/src_Snow_Core_Window.cpp$(ObjectSuffix): src/Snow/Core/Window.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Core_Window.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Core_Window.cpp$(DependSuffix) -MM src/Snow/Core/Window.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Core/Window.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Core_Window.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Core_Window.cpp$(PreprocessSuffix): src/Snow/Core/Window.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Core_Window.cpp$(PreprocessSuffix) src/Snow/Core/Window.cpp

$(IntermediateDirectory)/vendor_stb_stb_image.cpp$(ObjectSuffix): vendor/stb/stb_image.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/vendor_stb_stb_image.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/vendor_stb_stb_image.cpp$(DependSuffix) -MM vendor/stb/stb_image.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/vendor/stb/stb_image.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/vendor_stb_stb_image.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/vendor_stb_stb_image.cpp$(PreprocessSuffix): vendor/stb/stb_image.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/vendor_stb_stb_image.cpp$(PreprocessSuffix) vendor/stb/stb_image.cpp

$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLContext.cpp$(ObjectSuffix): src/Snow/Platform/OpenGL/OpenGLContext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLContext.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLContext.cpp$(DependSuffix) -MM src/Snow/Platform/OpenGL/OpenGLContext.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/OpenGL/OpenGLContext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLContext.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLContext.cpp$(PreprocessSuffix): src/Snow/Platform/OpenGL/OpenGLContext.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLContext.cpp$(PreprocessSuffix) src/Snow/Platform/OpenGL/OpenGLContext.cpp

$(IntermediateDirectory)/src_Snow_Core_Input.cpp$(ObjectSuffix): src/Snow/Core/Input.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Core_Input.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Core_Input.cpp$(DependSuffix) -MM src/Snow/Core/Input.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Core/Input.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Core_Input.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Core_Input.cpp$(PreprocessSuffix): src/Snow/Core/Input.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Core_Input.cpp$(PreprocessSuffix) src/Snow/Core/Input.cpp

$(IntermediateDirectory)/src_Snow_ImGui_ImGuiLayer.cpp$(ObjectSuffix): src/Snow/ImGui/ImGuiLayer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_ImGui_ImGuiLayer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_ImGui_ImGuiLayer.cpp$(DependSuffix) -MM src/Snow/ImGui/ImGuiLayer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/ImGui/ImGuiLayer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_ImGui_ImGuiLayer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_ImGui_ImGuiLayer.cpp$(PreprocessSuffix): src/Snow/ImGui/ImGuiLayer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_ImGui_ImGuiLayer.cpp$(PreprocessSuffix) src/Snow/ImGui/ImGuiLayer.cpp

$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLRenderCommand.cpp$(ObjectSuffix): src/Snow/Platform/OpenGL/OpenGLRenderCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLRenderCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLRenderCommand.cpp$(DependSuffix) -MM src/Snow/Platform/OpenGL/OpenGLRenderCommand.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/OpenGL/OpenGLRenderCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLRenderCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLRenderCommand.cpp$(PreprocessSuffix): src/Snow/Platform/OpenGL/OpenGLRenderCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLRenderCommand.cpp$(PreprocessSuffix) src/Snow/Platform/OpenGL/OpenGLRenderCommand.cpp

$(IntermediateDirectory)/src_Snow_Core_Log.cpp$(ObjectSuffix): src/Snow/Core/Log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Core_Log.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Core_Log.cpp$(DependSuffix) -MM src/Snow/Core/Log.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Core/Log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Core_Log.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Core_Log.cpp$(PreprocessSuffix): src/Snow/Core/Log.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Core_Log.cpp$(PreprocessSuffix) src/Snow/Core/Log.cpp

$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLBuffer.cpp$(ObjectSuffix): src/Snow/Platform/OpenGL/OpenGLBuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLBuffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLBuffer.cpp$(DependSuffix) -MM src/Snow/Platform/OpenGL/OpenGLBuffer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/OpenGL/OpenGLBuffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLBuffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLBuffer.cpp$(PreprocessSuffix): src/Snow/Platform/OpenGL/OpenGLBuffer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLBuffer.cpp$(PreprocessSuffix) src/Snow/Platform/OpenGL/OpenGLBuffer.cpp

$(IntermediateDirectory)/src_Snow_Render_Pipeline.cpp$(ObjectSuffix): src/Snow/Render/Pipeline.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_Pipeline.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_Pipeline.cpp$(DependSuffix) -MM src/Snow/Render/Pipeline.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/Pipeline.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_Pipeline.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_Pipeline.cpp$(PreprocessSuffix): src/Snow/Render/Pipeline.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_Pipeline.cpp$(PreprocessSuffix) src/Snow/Render/Pipeline.cpp

$(IntermediateDirectory)/src_Snow_Math_Math.cpp$(ObjectSuffix): src/Snow/Math/Math.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Math_Math.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Math_Math.cpp$(DependSuffix) -MM src/Snow/Math/Math.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Math/Math.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Math_Math.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Math_Math.cpp$(PreprocessSuffix): src/Snow/Math/Math.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Math_Math.cpp$(PreprocessSuffix) src/Snow/Math/Math.cpp

$(IntermediateDirectory)/src_Snow_Core_Layer.cpp$(ObjectSuffix): src/Snow/Core/Layer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Core_Layer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Core_Layer.cpp$(DependSuffix) -MM src/Snow/Core/Layer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Core/Layer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Core_Layer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Core_Layer.cpp$(PreprocessSuffix): src/Snow/Core/Layer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Core_Layer.cpp$(PreprocessSuffix) src/Snow/Core/Layer.cpp

$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLTexture.cpp$(ObjectSuffix): src/Snow/Platform/OpenGL/OpenGLTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLTexture.cpp$(DependSuffix) -MM src/Snow/Platform/OpenGL/OpenGLTexture.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/OpenGL/OpenGLTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLTexture.cpp$(PreprocessSuffix): src/Snow/Platform/OpenGL/OpenGLTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLTexture.cpp$(PreprocessSuffix) src/Snow/Platform/OpenGL/OpenGLTexture.cpp

$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanDevice.cpp$(ObjectSuffix): src/Snow/Platform/Vulkan/VulkanDevice.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanDevice.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanDevice.cpp$(DependSuffix) -MM src/Snow/Platform/Vulkan/VulkanDevice.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Vulkan/VulkanDevice.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanDevice.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanDevice.cpp$(PreprocessSuffix): src/Snow/Platform/Vulkan/VulkanDevice.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanDevice.cpp$(PreprocessSuffix) src/Snow/Platform/Vulkan/VulkanDevice.cpp

$(IntermediateDirectory)/src_Snow_Core_Application.cpp$(ObjectSuffix): src/Snow/Core/Application.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Core_Application.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Core_Application.cpp$(DependSuffix) -MM src/Snow/Core/Application.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Core/Application.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Core_Application.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Core_Application.cpp$(PreprocessSuffix): src/Snow/Core/Application.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Core_Application.cpp$(PreprocessSuffix) src/Snow/Core/Application.cpp

$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanPipeline.cpp$(ObjectSuffix): src/Snow/Platform/Vulkan/VulkanPipeline.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanPipeline.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanPipeline.cpp$(DependSuffix) -MM src/Snow/Platform/Vulkan/VulkanPipeline.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Vulkan/VulkanPipeline.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanPipeline.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanPipeline.cpp$(PreprocessSuffix): src/Snow/Platform/Vulkan/VulkanPipeline.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanPipeline.cpp$(PreprocessSuffix) src/Snow/Platform/Vulkan/VulkanPipeline.cpp

$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxInput.cpp$(ObjectSuffix): src/Snow/Platform/Linux/LinuxInput.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxInput.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxInput.cpp$(DependSuffix) -MM src/Snow/Platform/Linux/LinuxInput.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Linux/LinuxInput.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxInput.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxInput.cpp$(PreprocessSuffix): src/Snow/Platform/Linux/LinuxInput.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxInput.cpp$(PreprocessSuffix) src/Snow/Platform/Linux/LinuxInput.cpp

$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxWindow.cpp$(ObjectSuffix): src/Snow/Platform/Linux/LinuxWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxWindow.cpp$(DependSuffix) -MM src/Snow/Platform/Linux/LinuxWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Linux/LinuxWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxWindow.cpp$(PreprocessSuffix): src/Snow/Platform/Linux/LinuxWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Linux_LinuxWindow.cpp$(PreprocessSuffix) src/Snow/Platform/Linux/LinuxWindow.cpp

$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLImGuiLayer.cpp$(ObjectSuffix): src/Snow/Platform/OpenGL/OpenGLImGuiLayer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLImGuiLayer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLImGuiLayer.cpp$(DependSuffix) -MM src/Snow/Platform/OpenGL/OpenGLImGuiLayer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/OpenGL/OpenGLImGuiLayer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLImGuiLayer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLImGuiLayer.cpp$(PreprocessSuffix): src/Snow/Platform/OpenGL/OpenGLImGuiLayer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLImGuiLayer.cpp$(PreprocessSuffix) src/Snow/Platform/OpenGL/OpenGLImGuiLayer.cpp

$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLPipeline.cpp$(ObjectSuffix): src/Snow/Platform/OpenGL/OpenGLPipeline.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLPipeline.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLPipeline.cpp$(DependSuffix) -MM src/Snow/Platform/OpenGL/OpenGLPipeline.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/OpenGL/OpenGLPipeline.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLPipeline.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLPipeline.cpp$(PreprocessSuffix): src/Snow/Platform/OpenGL/OpenGLPipeline.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLPipeline.cpp$(PreprocessSuffix) src/Snow/Platform/OpenGL/OpenGLPipeline.cpp

$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLSwapChain.cpp$(ObjectSuffix): src/Snow/Platform/OpenGL/OpenGLSwapChain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLSwapChain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLSwapChain.cpp$(DependSuffix) -MM src/Snow/Platform/OpenGL/OpenGLSwapChain.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/OpenGL/OpenGLSwapChain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLSwapChain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLSwapChain.cpp$(PreprocessSuffix): src/Snow/Platform/OpenGL/OpenGLSwapChain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_OpenGL_OpenGLSwapChain.cpp$(PreprocessSuffix) src/Snow/Platform/OpenGL/OpenGLSwapChain.cpp

$(IntermediateDirectory)/src_Snow_Render_RenderPass.cpp$(ObjectSuffix): src/Snow/Render/RenderPass.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Render_RenderPass.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Render_RenderPass.cpp$(DependSuffix) -MM src/Snow/Render/RenderPass.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Render/RenderPass.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Render_RenderPass.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Render_RenderPass.cpp$(PreprocessSuffix): src/Snow/Render/RenderPass.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Render_RenderPass.cpp$(PreprocessSuffix) src/Snow/Render/RenderPass.cpp

$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanContext.cpp$(ObjectSuffix): src/Snow/Platform/Vulkan/VulkanContext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanContext.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanContext.cpp$(DependSuffix) -MM src/Snow/Platform/Vulkan/VulkanContext.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Vulkan/VulkanContext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanContext.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanContext.cpp$(PreprocessSuffix): src/Snow/Platform/Vulkan/VulkanContext.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanContext.cpp$(PreprocessSuffix) src/Snow/Platform/Vulkan/VulkanContext.cpp

$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanImGuiLayer.cpp$(ObjectSuffix): src/Snow/Platform/Vulkan/VulkanImGuiLayer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanImGuiLayer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanImGuiLayer.cpp$(DependSuffix) -MM src/Snow/Platform/Vulkan/VulkanImGuiLayer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Vulkan/VulkanImGuiLayer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanImGuiLayer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanImGuiLayer.cpp$(PreprocessSuffix): src/Snow/Platform/Vulkan/VulkanImGuiLayer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanImGuiLayer.cpp$(PreprocessSuffix) src/Snow/Platform/Vulkan/VulkanImGuiLayer.cpp

$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanRenderCommand.cpp$(ObjectSuffix): src/Snow/Platform/Vulkan/VulkanRenderCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanRenderCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanRenderCommand.cpp$(DependSuffix) -MM src/Snow/Platform/Vulkan/VulkanRenderCommand.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Snow/src/Snow/Platform/Vulkan/VulkanRenderCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanRenderCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanRenderCommand.cpp$(PreprocessSuffix): src/Snow/Platform/Vulkan/VulkanRenderCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Snow_Platform_Vulkan_VulkanRenderCommand.cpp$(PreprocessSuffix) src/Snow/Platform/Vulkan/VulkanRenderCommand.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ../bin-int/Debug-linux-x86_64/Snow/


