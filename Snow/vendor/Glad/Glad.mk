##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Glad
ConfigurationName      :=Debug
WorkspacePath          :=/media/james/Dev/Snow
ProjectPath            :=/media/james/Dev/Snow/Snow/vendor/Glad
IntermediateDirectory  :=bin-int/Debug-linux-x86_64/Glad
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
OutputFile             :=bin/Debug-linux-x86_64/Glad/libGlad.a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Glad.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -m64
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -m64 $(Preprocessors)
CFLAGS   :=  -m64 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_glad_glx.c$(ObjectSuffix) $(IntermediateDirectory)/src_glad.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList)
	@$(MakeDirCommand) "/media/james/Dev/Snow/.build-debug"
	@echo rebuilt > "/media/james/Dev/Snow/.build-debug/Glad"

MakeIntermediateDirs:
	@test -d bin-int/Debug-linux-x86_64/Glad || $(MakeDirCommand) bin-int/Debug-linux-x86_64/Glad


bin-int/Debug-linux-x86_64/Glad:
	@test -d bin-int/Debug-linux-x86_64/Glad || $(MakeDirCommand) bin-int/Debug-linux-x86_64/Glad

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_glad_glx.c$(ObjectSuffix): src/glad_glx.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_glad_glx.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_glad_glx.c$(DependSuffix) -MM src/glad_glx.c
	$(CC) $(SourceSwitch) "/media/james/Dev/Snow/Snow/vendor/Glad/src/glad_glx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_glad_glx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_glad_glx.c$(PreprocessSuffix): src/glad_glx.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_glad_glx.c$(PreprocessSuffix) src/glad_glx.c

$(IntermediateDirectory)/src_glad.c$(ObjectSuffix): src/glad.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_glad.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_glad.c$(DependSuffix) -MM src/glad.c
	$(CC) $(SourceSwitch) "/media/james/Dev/Snow/Snow/vendor/Glad/src/glad.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_glad.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_glad.c$(PreprocessSuffix): src/glad.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_glad.c$(PreprocessSuffix) src/glad.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r bin-int/Debug-linux-x86_64/Glad/


