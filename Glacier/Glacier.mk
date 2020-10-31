##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Glacier
ConfigurationName      :=Debug
WorkspacePath          :=/media/james/Dev/Snow
ProjectPath            :=/media/james/Dev/Snow/Glacier
IntermediateDirectory  :=../bin-int/Debug-linux-x86_64/Glacier
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
OutputFile             :=../bin/Debug-linux-x86_64/Glacier/Glacier
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Glacier.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -m64 ../bin/Debug-linux-x86_64/Snow/libSnow.a ../Snow/vendor/Glad/bin/Debug-linux-x86_64/Glad/libGlad.a ../Snow/vendor/imgui/bin/Debug-linux-x86_64/ImGui/libImGui.a ../Snow/vendor/GLFW/bin/Debug-linux-x86_64/GLFW/libGLFW.a ../Snow/vendor/SPIRV-Cross/bin/Debug-linux-x86_64/SPIRVCross/libSPIRVCross.a -lX11 -lXi -lxcb -lpthread -ldl -lvulkan -lshaderc -lshaderc_util -lglslang -lMachineIndependent -lSPIRV -lOGLCompiler -lOSDependent -lGenericCodeGen -lSPIRV-Tools-opt -lSPIRV-Tools
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)src $(IncludeSwitch)../Snow/src $(IncludeSwitch)../Snow/vendor/spdlog/include $(IncludeSwitch)../Snow/vendor/imgui $(IncludeSwitch)../Snow/vendor/vulkan/Include/Linux $(IncludeSwitch)../Snow/vendor/shaderc/Include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../Snow/vendor/vulkan/Lib/Linux $(LibraryPathSwitch)../Snow/vendor/shaderc/Lib/Linux $(LibraryPathSwitch)../Snow/vendor/glslang/Lib/Linux $(LibraryPathSwitch)../Snow/vendor/SPIRV-Tools/Lib/Linux 

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
Objects0=$(IntermediateDirectory)/src_EditorLayer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Application.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d "../.build-debug/Snow" "../.build-debug/Glad" "../.build-debug/ImGui" "../.build-debug/GLFW" "../.build-debug/SPIRVCross" $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

"../.build-debug/Snow":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/Snow"


"../.build-debug/Glad":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/Glad"


"../.build-debug/ImGui":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/ImGui"


"../.build-debug/GLFW":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/GLFW"


"../.build-debug/SPIRVCross":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/SPIRVCross"




MakeIntermediateDirs:
	@test -d ../bin-int/Debug-linux-x86_64/Glacier || $(MakeDirCommand) ../bin-int/Debug-linux-x86_64/Glacier


$(IntermediateDirectory)/.d:
	@test -d ../bin-int/Debug-linux-x86_64/Glacier || $(MakeDirCommand) ../bin-int/Debug-linux-x86_64/Glacier

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_EditorLayer.cpp$(ObjectSuffix): src/EditorLayer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_EditorLayer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_EditorLayer.cpp$(DependSuffix) -MM src/EditorLayer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Glacier/src/EditorLayer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_EditorLayer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_EditorLayer.cpp$(PreprocessSuffix): src/EditorLayer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_EditorLayer.cpp$(PreprocessSuffix) src/EditorLayer.cpp

$(IntermediateDirectory)/src_Application.cpp$(ObjectSuffix): src/Application.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Application.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Application.cpp$(DependSuffix) -MM src/Application.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/james/Dev/Snow/Glacier/src/Application.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Application.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Application.cpp$(PreprocessSuffix): src/Application.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Application.cpp$(PreprocessSuffix) src/Application.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ../bin-int/Debug-linux-x86_64/Glacier/


