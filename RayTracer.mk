##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=RayTracer
ConfigurationName      :=Debug
WorkspacePath          :=/Users/zhuqian/ClionProjects/RayTracer
ProjectPath            :=/Users/zhuqian/ClionProjects/RayTracer
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=诸谦
Date                   :=07/11/2016
CodeLitePath           :="/Users/zhuqian/Library/Application Support/codelite"
LinkerName             :=/usr/bin/clang++
SharedObjectLinkerName :=/usr/bin/clang++ -dynamiclib -fPIC
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RayTracer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)src $(IncludeSwitch)src/core 
IncludePCH             := 
RcIncludePath          := $(IncludeSwitch)../. 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/clang++
CC       := /usr/bin/clang
CXXFLAGS :=  -std=c++11 -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/private/var/folders/dn/0zh043g96vb05vtvc182qzf00000gn/T/AppTranslocation/F4270BCC-C263-4265-92C0-9198E23C3D60/d/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_accelerator_bvh.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_accelerator_grid.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_accelerator_normal.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_camera_orthographic.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_camera_perspective.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_camera_PinholeCamera.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_BRDF.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_diffgeom.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_core_geometry.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_global.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_imageio.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_integrator.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_Intersection.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_light.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_material.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_memory.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_mipmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_montecarlo.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_core_parallel.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_primitive.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_random.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_reflection.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_renderer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_RGB.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_sampler.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_Scene.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_shape.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_core_texture.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_core_transform.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_film_png.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_film_ppm.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_filter_box.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_integrator_directlight.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_integrator_ic.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_integrator_igi.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_integrator_path.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_integrator_SimpleIntegrator.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_light_distant.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_light_point.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_light_spot.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_material_grass.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_material_matte.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_material_metal.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_material_mirror.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_material_translucent.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_renderer_metropolis.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/src_renderer_simpleRenderer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_sampler_lowdiscrepancy.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_sampler_randomSampler.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_sampler_stratified.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shape_sphere.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shape_trianglemesh.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_test_test.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_texture_image.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_tool_mesh.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_thrid_lodepng_lodepng.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_thrid_objloader_Model.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_thrid_tinyobjloader_tiny_obj_loader.cc$(ObjectSuffix) \
	



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM src/main.cpp

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix)src/main.cpp

$(IntermediateDirectory)/src_accelerator_bvh.cpp$(ObjectSuffix): src/accelerator/bvh.cpp $(IntermediateDirectory)/src_accelerator_bvh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/accelerator/bvh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_accelerator_bvh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_accelerator_bvh.cpp$(DependSuffix): src/accelerator/bvh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_accelerator_bvh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_accelerator_bvh.cpp$(DependSuffix) -MM src/accelerator/bvh.cpp

$(IntermediateDirectory)/src_accelerator_bvh.cpp$(PreprocessSuffix): src/accelerator/bvh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_accelerator_bvh.cpp$(PreprocessSuffix)src/accelerator/bvh.cpp

$(IntermediateDirectory)/src_accelerator_grid.cpp$(ObjectSuffix): src/accelerator/grid.cpp $(IntermediateDirectory)/src_accelerator_grid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/accelerator/grid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_accelerator_grid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_accelerator_grid.cpp$(DependSuffix): src/accelerator/grid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_accelerator_grid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_accelerator_grid.cpp$(DependSuffix) -MM src/accelerator/grid.cpp

$(IntermediateDirectory)/src_accelerator_grid.cpp$(PreprocessSuffix): src/accelerator/grid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_accelerator_grid.cpp$(PreprocessSuffix)src/accelerator/grid.cpp

$(IntermediateDirectory)/src_accelerator_normal.cpp$(ObjectSuffix): src/accelerator/normal.cpp $(IntermediateDirectory)/src_accelerator_normal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/accelerator/normal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_accelerator_normal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_accelerator_normal.cpp$(DependSuffix): src/accelerator/normal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_accelerator_normal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_accelerator_normal.cpp$(DependSuffix) -MM src/accelerator/normal.cpp

$(IntermediateDirectory)/src_accelerator_normal.cpp$(PreprocessSuffix): src/accelerator/normal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_accelerator_normal.cpp$(PreprocessSuffix)src/accelerator/normal.cpp

$(IntermediateDirectory)/src_camera_orthographic.cpp$(ObjectSuffix): src/camera/orthographic.cpp $(IntermediateDirectory)/src_camera_orthographic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/camera/orthographic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_camera_orthographic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_camera_orthographic.cpp$(DependSuffix): src/camera/orthographic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_camera_orthographic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_camera_orthographic.cpp$(DependSuffix) -MM src/camera/orthographic.cpp

$(IntermediateDirectory)/src_camera_orthographic.cpp$(PreprocessSuffix): src/camera/orthographic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_camera_orthographic.cpp$(PreprocessSuffix)src/camera/orthographic.cpp

$(IntermediateDirectory)/src_camera_perspective.cpp$(ObjectSuffix): src/camera/perspective.cpp $(IntermediateDirectory)/src_camera_perspective.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/camera/perspective.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_camera_perspective.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_camera_perspective.cpp$(DependSuffix): src/camera/perspective.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_camera_perspective.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_camera_perspective.cpp$(DependSuffix) -MM src/camera/perspective.cpp

$(IntermediateDirectory)/src_camera_perspective.cpp$(PreprocessSuffix): src/camera/perspective.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_camera_perspective.cpp$(PreprocessSuffix)src/camera/perspective.cpp

$(IntermediateDirectory)/src_camera_PinholeCamera.cpp$(ObjectSuffix): src/camera/PinholeCamera.cpp $(IntermediateDirectory)/src_camera_PinholeCamera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/camera/PinholeCamera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_camera_PinholeCamera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_camera_PinholeCamera.cpp$(DependSuffix): src/camera/PinholeCamera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_camera_PinholeCamera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_camera_PinholeCamera.cpp$(DependSuffix) -MM src/camera/PinholeCamera.cpp

$(IntermediateDirectory)/src_camera_PinholeCamera.cpp$(PreprocessSuffix): src/camera/PinholeCamera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_camera_PinholeCamera.cpp$(PreprocessSuffix)src/camera/PinholeCamera.cpp

$(IntermediateDirectory)/src_core_BRDF.cpp$(ObjectSuffix): src/core/BRDF.cpp $(IntermediateDirectory)/src_core_BRDF.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/BRDF.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_BRDF.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_BRDF.cpp$(DependSuffix): src/core/BRDF.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_BRDF.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_BRDF.cpp$(DependSuffix) -MM src/core/BRDF.cpp

$(IntermediateDirectory)/src_core_BRDF.cpp$(PreprocessSuffix): src/core/BRDF.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_BRDF.cpp$(PreprocessSuffix)src/core/BRDF.cpp

$(IntermediateDirectory)/src_core_Camera.cpp$(ObjectSuffix): src/core/Camera.cpp $(IntermediateDirectory)/src_core_Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_Camera.cpp$(DependSuffix): src/core/Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_Camera.cpp$(DependSuffix) -MM src/core/Camera.cpp

$(IntermediateDirectory)/src_core_Camera.cpp$(PreprocessSuffix): src/core/Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_Camera.cpp$(PreprocessSuffix)src/core/Camera.cpp

$(IntermediateDirectory)/src_core_diffgeom.cpp$(ObjectSuffix): src/core/diffgeom.cpp $(IntermediateDirectory)/src_core_diffgeom.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/diffgeom.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_diffgeom.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_diffgeom.cpp$(DependSuffix): src/core/diffgeom.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_diffgeom.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_diffgeom.cpp$(DependSuffix) -MM src/core/diffgeom.cpp

$(IntermediateDirectory)/src_core_diffgeom.cpp$(PreprocessSuffix): src/core/diffgeom.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_diffgeom.cpp$(PreprocessSuffix)src/core/diffgeom.cpp

$(IntermediateDirectory)/src_core_geometry.cpp$(ObjectSuffix): src/core/geometry.cpp $(IntermediateDirectory)/src_core_geometry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/geometry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_geometry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_geometry.cpp$(DependSuffix): src/core/geometry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_geometry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_geometry.cpp$(DependSuffix) -MM src/core/geometry.cpp

$(IntermediateDirectory)/src_core_geometry.cpp$(PreprocessSuffix): src/core/geometry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_geometry.cpp$(PreprocessSuffix)src/core/geometry.cpp

$(IntermediateDirectory)/src_core_global.cpp$(ObjectSuffix): src/core/global.cpp $(IntermediateDirectory)/src_core_global.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/global.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_global.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_global.cpp$(DependSuffix): src/core/global.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_global.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_global.cpp$(DependSuffix) -MM src/core/global.cpp

$(IntermediateDirectory)/src_core_global.cpp$(PreprocessSuffix): src/core/global.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_global.cpp$(PreprocessSuffix)src/core/global.cpp

$(IntermediateDirectory)/src_core_imageio.cpp$(ObjectSuffix): src/core/imageio.cpp $(IntermediateDirectory)/src_core_imageio.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/imageio.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_imageio.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_imageio.cpp$(DependSuffix): src/core/imageio.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_imageio.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_imageio.cpp$(DependSuffix) -MM src/core/imageio.cpp

$(IntermediateDirectory)/src_core_imageio.cpp$(PreprocessSuffix): src/core/imageio.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_imageio.cpp$(PreprocessSuffix)src/core/imageio.cpp

$(IntermediateDirectory)/src_core_integrator.cpp$(ObjectSuffix): src/core/integrator.cpp $(IntermediateDirectory)/src_core_integrator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/integrator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_integrator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_integrator.cpp$(DependSuffix): src/core/integrator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_integrator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_integrator.cpp$(DependSuffix) -MM src/core/integrator.cpp

$(IntermediateDirectory)/src_core_integrator.cpp$(PreprocessSuffix): src/core/integrator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_integrator.cpp$(PreprocessSuffix)src/core/integrator.cpp

$(IntermediateDirectory)/src_core_Intersection.cpp$(ObjectSuffix): src/core/Intersection.cpp $(IntermediateDirectory)/src_core_Intersection.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/Intersection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_Intersection.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_Intersection.cpp$(DependSuffix): src/core/Intersection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_Intersection.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_Intersection.cpp$(DependSuffix) -MM src/core/Intersection.cpp

$(IntermediateDirectory)/src_core_Intersection.cpp$(PreprocessSuffix): src/core/Intersection.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_Intersection.cpp$(PreprocessSuffix)src/core/Intersection.cpp

$(IntermediateDirectory)/src_core_light.cpp$(ObjectSuffix): src/core/light.cpp $(IntermediateDirectory)/src_core_light.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/light.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_light.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_light.cpp$(DependSuffix): src/core/light.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_light.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_light.cpp$(DependSuffix) -MM src/core/light.cpp

$(IntermediateDirectory)/src_core_light.cpp$(PreprocessSuffix): src/core/light.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_light.cpp$(PreprocessSuffix)src/core/light.cpp

$(IntermediateDirectory)/src_core_material.cpp$(ObjectSuffix): src/core/material.cpp $(IntermediateDirectory)/src_core_material.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/material.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_material.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_material.cpp$(DependSuffix): src/core/material.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_material.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_material.cpp$(DependSuffix) -MM src/core/material.cpp

$(IntermediateDirectory)/src_core_material.cpp$(PreprocessSuffix): src/core/material.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_material.cpp$(PreprocessSuffix)src/core/material.cpp

$(IntermediateDirectory)/src_core_memory.cpp$(ObjectSuffix): src/core/memory.cpp $(IntermediateDirectory)/src_core_memory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/memory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_memory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_memory.cpp$(DependSuffix): src/core/memory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_memory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_memory.cpp$(DependSuffix) -MM src/core/memory.cpp

$(IntermediateDirectory)/src_core_memory.cpp$(PreprocessSuffix): src/core/memory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_memory.cpp$(PreprocessSuffix)src/core/memory.cpp

$(IntermediateDirectory)/src_core_mipmap.cpp$(ObjectSuffix): src/core/mipmap.cpp $(IntermediateDirectory)/src_core_mipmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/mipmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_mipmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_mipmap.cpp$(DependSuffix): src/core/mipmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_mipmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_mipmap.cpp$(DependSuffix) -MM src/core/mipmap.cpp

$(IntermediateDirectory)/src_core_mipmap.cpp$(PreprocessSuffix): src/core/mipmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_mipmap.cpp$(PreprocessSuffix)src/core/mipmap.cpp

$(IntermediateDirectory)/src_core_montecarlo.cpp$(ObjectSuffix): src/core/montecarlo.cpp $(IntermediateDirectory)/src_core_montecarlo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/montecarlo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_montecarlo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_montecarlo.cpp$(DependSuffix): src/core/montecarlo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_montecarlo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_montecarlo.cpp$(DependSuffix) -MM src/core/montecarlo.cpp

$(IntermediateDirectory)/src_core_montecarlo.cpp$(PreprocessSuffix): src/core/montecarlo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_montecarlo.cpp$(PreprocessSuffix)src/core/montecarlo.cpp

$(IntermediateDirectory)/src_core_parallel.cpp$(ObjectSuffix): src/core/parallel.cpp $(IntermediateDirectory)/src_core_parallel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/parallel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_parallel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_parallel.cpp$(DependSuffix): src/core/parallel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_parallel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_parallel.cpp$(DependSuffix) -MM src/core/parallel.cpp

$(IntermediateDirectory)/src_core_parallel.cpp$(PreprocessSuffix): src/core/parallel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_parallel.cpp$(PreprocessSuffix)src/core/parallel.cpp

$(IntermediateDirectory)/src_core_primitive.cpp$(ObjectSuffix): src/core/primitive.cpp $(IntermediateDirectory)/src_core_primitive.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/primitive.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_primitive.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_primitive.cpp$(DependSuffix): src/core/primitive.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_primitive.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_primitive.cpp$(DependSuffix) -MM src/core/primitive.cpp

$(IntermediateDirectory)/src_core_primitive.cpp$(PreprocessSuffix): src/core/primitive.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_primitive.cpp$(PreprocessSuffix)src/core/primitive.cpp

$(IntermediateDirectory)/src_core_random.cpp$(ObjectSuffix): src/core/random.cpp $(IntermediateDirectory)/src_core_random.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/random.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_random.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_random.cpp$(DependSuffix): src/core/random.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_random.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_random.cpp$(DependSuffix) -MM src/core/random.cpp

$(IntermediateDirectory)/src_core_random.cpp$(PreprocessSuffix): src/core/random.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_random.cpp$(PreprocessSuffix)src/core/random.cpp

$(IntermediateDirectory)/src_core_reflection.cpp$(ObjectSuffix): src/core/reflection.cpp $(IntermediateDirectory)/src_core_reflection.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/reflection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_reflection.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_reflection.cpp$(DependSuffix): src/core/reflection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_reflection.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_reflection.cpp$(DependSuffix) -MM src/core/reflection.cpp

$(IntermediateDirectory)/src_core_reflection.cpp$(PreprocessSuffix): src/core/reflection.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_reflection.cpp$(PreprocessSuffix)src/core/reflection.cpp

$(IntermediateDirectory)/src_core_renderer.cpp$(ObjectSuffix): src/core/renderer.cpp $(IntermediateDirectory)/src_core_renderer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/renderer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_renderer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_renderer.cpp$(DependSuffix): src/core/renderer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_renderer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_renderer.cpp$(DependSuffix) -MM src/core/renderer.cpp

$(IntermediateDirectory)/src_core_renderer.cpp$(PreprocessSuffix): src/core/renderer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_renderer.cpp$(PreprocessSuffix)src/core/renderer.cpp

$(IntermediateDirectory)/src_core_RGB.cpp$(ObjectSuffix): src/core/RGB.cpp $(IntermediateDirectory)/src_core_RGB.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/RGB.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_RGB.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_RGB.cpp$(DependSuffix): src/core/RGB.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_RGB.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_RGB.cpp$(DependSuffix) -MM src/core/RGB.cpp

$(IntermediateDirectory)/src_core_RGB.cpp$(PreprocessSuffix): src/core/RGB.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_RGB.cpp$(PreprocessSuffix)src/core/RGB.cpp

$(IntermediateDirectory)/src_core_sampler.cpp$(ObjectSuffix): src/core/sampler.cpp $(IntermediateDirectory)/src_core_sampler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/sampler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_sampler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_sampler.cpp$(DependSuffix): src/core/sampler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_sampler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_sampler.cpp$(DependSuffix) -MM src/core/sampler.cpp

$(IntermediateDirectory)/src_core_sampler.cpp$(PreprocessSuffix): src/core/sampler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_sampler.cpp$(PreprocessSuffix)src/core/sampler.cpp

$(IntermediateDirectory)/src_core_Scene.cpp$(ObjectSuffix): src/core/Scene.cpp $(IntermediateDirectory)/src_core_Scene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/Scene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_Scene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_Scene.cpp$(DependSuffix): src/core/Scene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_Scene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_Scene.cpp$(DependSuffix) -MM src/core/Scene.cpp

$(IntermediateDirectory)/src_core_Scene.cpp$(PreprocessSuffix): src/core/Scene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_Scene.cpp$(PreprocessSuffix)src/core/Scene.cpp

$(IntermediateDirectory)/src_core_shape.cpp$(ObjectSuffix): src/core/shape.cpp $(IntermediateDirectory)/src_core_shape.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/shape.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_shape.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_shape.cpp$(DependSuffix): src/core/shape.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_shape.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_shape.cpp$(DependSuffix) -MM src/core/shape.cpp

$(IntermediateDirectory)/src_core_shape.cpp$(PreprocessSuffix): src/core/shape.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_shape.cpp$(PreprocessSuffix)src/core/shape.cpp

$(IntermediateDirectory)/src_core_texture.cpp$(ObjectSuffix): src/core/texture.cpp $(IntermediateDirectory)/src_core_texture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/texture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_texture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_texture.cpp$(DependSuffix): src/core/texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_texture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_texture.cpp$(DependSuffix) -MM src/core/texture.cpp

$(IntermediateDirectory)/src_core_texture.cpp$(PreprocessSuffix): src/core/texture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_texture.cpp$(PreprocessSuffix)src/core/texture.cpp

$(IntermediateDirectory)/src_core_transform.cpp$(ObjectSuffix): src/core/transform.cpp $(IntermediateDirectory)/src_core_transform.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/core/transform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_core_transform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_core_transform.cpp$(DependSuffix): src/core/transform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_core_transform.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_core_transform.cpp$(DependSuffix) -MM src/core/transform.cpp

$(IntermediateDirectory)/src_core_transform.cpp$(PreprocessSuffix): src/core/transform.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_core_transform.cpp$(PreprocessSuffix)src/core/transform.cpp

$(IntermediateDirectory)/src_film_png.cpp$(ObjectSuffix): src/film/png.cpp $(IntermediateDirectory)/src_film_png.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/film/png.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_film_png.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_film_png.cpp$(DependSuffix): src/film/png.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_film_png.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_film_png.cpp$(DependSuffix) -MM src/film/png.cpp

$(IntermediateDirectory)/src_film_png.cpp$(PreprocessSuffix): src/film/png.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_film_png.cpp$(PreprocessSuffix)src/film/png.cpp

$(IntermediateDirectory)/src_film_ppm.cpp$(ObjectSuffix): src/film/ppm.cpp $(IntermediateDirectory)/src_film_ppm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/film/ppm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_film_ppm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_film_ppm.cpp$(DependSuffix): src/film/ppm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_film_ppm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_film_ppm.cpp$(DependSuffix) -MM src/film/ppm.cpp

$(IntermediateDirectory)/src_film_ppm.cpp$(PreprocessSuffix): src/film/ppm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_film_ppm.cpp$(PreprocessSuffix)src/film/ppm.cpp

$(IntermediateDirectory)/src_filter_box.cpp$(ObjectSuffix): src/filter/box.cpp $(IntermediateDirectory)/src_filter_box.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/filter/box.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_filter_box.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_filter_box.cpp$(DependSuffix): src/filter/box.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_filter_box.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_filter_box.cpp$(DependSuffix) -MM src/filter/box.cpp

$(IntermediateDirectory)/src_filter_box.cpp$(PreprocessSuffix): src/filter/box.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_filter_box.cpp$(PreprocessSuffix)src/filter/box.cpp

$(IntermediateDirectory)/src_integrator_directlight.cpp$(ObjectSuffix): src/integrator/directlight.cpp $(IntermediateDirectory)/src_integrator_directlight.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/integrator/directlight.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_integrator_directlight.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_integrator_directlight.cpp$(DependSuffix): src/integrator/directlight.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_integrator_directlight.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_integrator_directlight.cpp$(DependSuffix) -MM src/integrator/directlight.cpp

$(IntermediateDirectory)/src_integrator_directlight.cpp$(PreprocessSuffix): src/integrator/directlight.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_integrator_directlight.cpp$(PreprocessSuffix)src/integrator/directlight.cpp

$(IntermediateDirectory)/src_integrator_ic.cpp$(ObjectSuffix): src/integrator/ic.cpp $(IntermediateDirectory)/src_integrator_ic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/integrator/ic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_integrator_ic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_integrator_ic.cpp$(DependSuffix): src/integrator/ic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_integrator_ic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_integrator_ic.cpp$(DependSuffix) -MM src/integrator/ic.cpp

$(IntermediateDirectory)/src_integrator_ic.cpp$(PreprocessSuffix): src/integrator/ic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_integrator_ic.cpp$(PreprocessSuffix)src/integrator/ic.cpp

$(IntermediateDirectory)/src_integrator_igi.cpp$(ObjectSuffix): src/integrator/igi.cpp $(IntermediateDirectory)/src_integrator_igi.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/integrator/igi.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_integrator_igi.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_integrator_igi.cpp$(DependSuffix): src/integrator/igi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_integrator_igi.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_integrator_igi.cpp$(DependSuffix) -MM src/integrator/igi.cpp

$(IntermediateDirectory)/src_integrator_igi.cpp$(PreprocessSuffix): src/integrator/igi.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_integrator_igi.cpp$(PreprocessSuffix)src/integrator/igi.cpp

$(IntermediateDirectory)/src_integrator_path.cpp$(ObjectSuffix): src/integrator/path.cpp $(IntermediateDirectory)/src_integrator_path.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/integrator/path.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_integrator_path.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_integrator_path.cpp$(DependSuffix): src/integrator/path.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_integrator_path.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_integrator_path.cpp$(DependSuffix) -MM src/integrator/path.cpp

$(IntermediateDirectory)/src_integrator_path.cpp$(PreprocessSuffix): src/integrator/path.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_integrator_path.cpp$(PreprocessSuffix)src/integrator/path.cpp

$(IntermediateDirectory)/src_integrator_SimpleIntegrator.cpp$(ObjectSuffix): src/integrator/SimpleIntegrator.cpp $(IntermediateDirectory)/src_integrator_SimpleIntegrator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/integrator/SimpleIntegrator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_integrator_SimpleIntegrator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_integrator_SimpleIntegrator.cpp$(DependSuffix): src/integrator/SimpleIntegrator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_integrator_SimpleIntegrator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_integrator_SimpleIntegrator.cpp$(DependSuffix) -MM src/integrator/SimpleIntegrator.cpp

$(IntermediateDirectory)/src_integrator_SimpleIntegrator.cpp$(PreprocessSuffix): src/integrator/SimpleIntegrator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_integrator_SimpleIntegrator.cpp$(PreprocessSuffix)src/integrator/SimpleIntegrator.cpp

$(IntermediateDirectory)/src_light_distant.cpp$(ObjectSuffix): src/light/distant.cpp $(IntermediateDirectory)/src_light_distant.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/light/distant.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_light_distant.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_light_distant.cpp$(DependSuffix): src/light/distant.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_light_distant.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_light_distant.cpp$(DependSuffix) -MM src/light/distant.cpp

$(IntermediateDirectory)/src_light_distant.cpp$(PreprocessSuffix): src/light/distant.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_light_distant.cpp$(PreprocessSuffix)src/light/distant.cpp

$(IntermediateDirectory)/src_light_point.cpp$(ObjectSuffix): src/light/point.cpp $(IntermediateDirectory)/src_light_point.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/light/point.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_light_point.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_light_point.cpp$(DependSuffix): src/light/point.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_light_point.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_light_point.cpp$(DependSuffix) -MM src/light/point.cpp

$(IntermediateDirectory)/src_light_point.cpp$(PreprocessSuffix): src/light/point.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_light_point.cpp$(PreprocessSuffix)src/light/point.cpp

$(IntermediateDirectory)/src_light_spot.cpp$(ObjectSuffix): src/light/spot.cpp $(IntermediateDirectory)/src_light_spot.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/light/spot.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_light_spot.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_light_spot.cpp$(DependSuffix): src/light/spot.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_light_spot.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_light_spot.cpp$(DependSuffix) -MM src/light/spot.cpp

$(IntermediateDirectory)/src_light_spot.cpp$(PreprocessSuffix): src/light/spot.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_light_spot.cpp$(PreprocessSuffix)src/light/spot.cpp

$(IntermediateDirectory)/src_material_grass.cpp$(ObjectSuffix): src/material/grass.cpp $(IntermediateDirectory)/src_material_grass.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/material/grass.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_material_grass.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_material_grass.cpp$(DependSuffix): src/material/grass.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_material_grass.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_material_grass.cpp$(DependSuffix) -MM src/material/grass.cpp

$(IntermediateDirectory)/src_material_grass.cpp$(PreprocessSuffix): src/material/grass.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_material_grass.cpp$(PreprocessSuffix)src/material/grass.cpp

$(IntermediateDirectory)/src_material_matte.cpp$(ObjectSuffix): src/material/matte.cpp $(IntermediateDirectory)/src_material_matte.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/material/matte.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_material_matte.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_material_matte.cpp$(DependSuffix): src/material/matte.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_material_matte.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_material_matte.cpp$(DependSuffix) -MM src/material/matte.cpp

$(IntermediateDirectory)/src_material_matte.cpp$(PreprocessSuffix): src/material/matte.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_material_matte.cpp$(PreprocessSuffix)src/material/matte.cpp

$(IntermediateDirectory)/src_material_metal.cpp$(ObjectSuffix): src/material/metal.cpp $(IntermediateDirectory)/src_material_metal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/material/metal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_material_metal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_material_metal.cpp$(DependSuffix): src/material/metal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_material_metal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_material_metal.cpp$(DependSuffix) -MM src/material/metal.cpp

$(IntermediateDirectory)/src_material_metal.cpp$(PreprocessSuffix): src/material/metal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_material_metal.cpp$(PreprocessSuffix)src/material/metal.cpp

$(IntermediateDirectory)/src_material_mirror.cpp$(ObjectSuffix): src/material/mirror.cpp $(IntermediateDirectory)/src_material_mirror.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/material/mirror.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_material_mirror.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_material_mirror.cpp$(DependSuffix): src/material/mirror.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_material_mirror.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_material_mirror.cpp$(DependSuffix) -MM src/material/mirror.cpp

$(IntermediateDirectory)/src_material_mirror.cpp$(PreprocessSuffix): src/material/mirror.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_material_mirror.cpp$(PreprocessSuffix)src/material/mirror.cpp

$(IntermediateDirectory)/src_material_translucent.cpp$(ObjectSuffix): src/material/translucent.cpp $(IntermediateDirectory)/src_material_translucent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/material/translucent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_material_translucent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_material_translucent.cpp$(DependSuffix): src/material/translucent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_material_translucent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_material_translucent.cpp$(DependSuffix) -MM src/material/translucent.cpp

$(IntermediateDirectory)/src_material_translucent.cpp$(PreprocessSuffix): src/material/translucent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_material_translucent.cpp$(PreprocessSuffix)src/material/translucent.cpp

$(IntermediateDirectory)/src_renderer_metropolis.cpp$(ObjectSuffix): src/renderer/metropolis.cpp $(IntermediateDirectory)/src_renderer_metropolis.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/renderer/metropolis.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_renderer_metropolis.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_renderer_metropolis.cpp$(DependSuffix): src/renderer/metropolis.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_renderer_metropolis.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_renderer_metropolis.cpp$(DependSuffix) -MM src/renderer/metropolis.cpp

$(IntermediateDirectory)/src_renderer_metropolis.cpp$(PreprocessSuffix): src/renderer/metropolis.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_renderer_metropolis.cpp$(PreprocessSuffix)src/renderer/metropolis.cpp

$(IntermediateDirectory)/src_renderer_simpleRenderer.cpp$(ObjectSuffix): src/renderer/simpleRenderer.cpp $(IntermediateDirectory)/src_renderer_simpleRenderer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/renderer/simpleRenderer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_renderer_simpleRenderer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_renderer_simpleRenderer.cpp$(DependSuffix): src/renderer/simpleRenderer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_renderer_simpleRenderer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_renderer_simpleRenderer.cpp$(DependSuffix) -MM src/renderer/simpleRenderer.cpp

$(IntermediateDirectory)/src_renderer_simpleRenderer.cpp$(PreprocessSuffix): src/renderer/simpleRenderer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_renderer_simpleRenderer.cpp$(PreprocessSuffix)src/renderer/simpleRenderer.cpp

$(IntermediateDirectory)/src_sampler_lowdiscrepancy.cpp$(ObjectSuffix): src/sampler/lowdiscrepancy.cpp $(IntermediateDirectory)/src_sampler_lowdiscrepancy.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/sampler/lowdiscrepancy.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_sampler_lowdiscrepancy.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_sampler_lowdiscrepancy.cpp$(DependSuffix): src/sampler/lowdiscrepancy.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_sampler_lowdiscrepancy.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_sampler_lowdiscrepancy.cpp$(DependSuffix) -MM src/sampler/lowdiscrepancy.cpp

$(IntermediateDirectory)/src_sampler_lowdiscrepancy.cpp$(PreprocessSuffix): src/sampler/lowdiscrepancy.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_sampler_lowdiscrepancy.cpp$(PreprocessSuffix)src/sampler/lowdiscrepancy.cpp

$(IntermediateDirectory)/src_sampler_randomSampler.cpp$(ObjectSuffix): src/sampler/randomSampler.cpp $(IntermediateDirectory)/src_sampler_randomSampler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/sampler/randomSampler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_sampler_randomSampler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_sampler_randomSampler.cpp$(DependSuffix): src/sampler/randomSampler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_sampler_randomSampler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_sampler_randomSampler.cpp$(DependSuffix) -MM src/sampler/randomSampler.cpp

$(IntermediateDirectory)/src_sampler_randomSampler.cpp$(PreprocessSuffix): src/sampler/randomSampler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_sampler_randomSampler.cpp$(PreprocessSuffix)src/sampler/randomSampler.cpp

$(IntermediateDirectory)/src_sampler_stratified.cpp$(ObjectSuffix): src/sampler/stratified.cpp $(IntermediateDirectory)/src_sampler_stratified.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/sampler/stratified.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_sampler_stratified.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_sampler_stratified.cpp$(DependSuffix): src/sampler/stratified.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_sampler_stratified.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_sampler_stratified.cpp$(DependSuffix) -MM src/sampler/stratified.cpp

$(IntermediateDirectory)/src_sampler_stratified.cpp$(PreprocessSuffix): src/sampler/stratified.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_sampler_stratified.cpp$(PreprocessSuffix)src/sampler/stratified.cpp

$(IntermediateDirectory)/src_shape_sphere.cpp$(ObjectSuffix): src/shape/sphere.cpp $(IntermediateDirectory)/src_shape_sphere.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/shape/sphere.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shape_sphere.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shape_sphere.cpp$(DependSuffix): src/shape/sphere.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shape_sphere.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shape_sphere.cpp$(DependSuffix) -MM src/shape/sphere.cpp

$(IntermediateDirectory)/src_shape_sphere.cpp$(PreprocessSuffix): src/shape/sphere.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shape_sphere.cpp$(PreprocessSuffix)src/shape/sphere.cpp

$(IntermediateDirectory)/src_shape_trianglemesh.cpp$(ObjectSuffix): src/shape/trianglemesh.cpp $(IntermediateDirectory)/src_shape_trianglemesh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/shape/trianglemesh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shape_trianglemesh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shape_trianglemesh.cpp$(DependSuffix): src/shape/trianglemesh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shape_trianglemesh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shape_trianglemesh.cpp$(DependSuffix) -MM src/shape/trianglemesh.cpp

$(IntermediateDirectory)/src_shape_trianglemesh.cpp$(PreprocessSuffix): src/shape/trianglemesh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shape_trianglemesh.cpp$(PreprocessSuffix)src/shape/trianglemesh.cpp

$(IntermediateDirectory)/src_test_test.cpp$(ObjectSuffix): src/test/test.cpp $(IntermediateDirectory)/src_test_test.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/test/test.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_test_test.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_test_test.cpp$(DependSuffix): src/test/test.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_test_test.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_test_test.cpp$(DependSuffix) -MM src/test/test.cpp

$(IntermediateDirectory)/src_test_test.cpp$(PreprocessSuffix): src/test/test.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_test_test.cpp$(PreprocessSuffix)src/test/test.cpp

$(IntermediateDirectory)/src_texture_image.cpp$(ObjectSuffix): src/texture/image.cpp $(IntermediateDirectory)/src_texture_image.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/texture/image.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_texture_image.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_texture_image.cpp$(DependSuffix): src/texture/image.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_texture_image.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_texture_image.cpp$(DependSuffix) -MM src/texture/image.cpp

$(IntermediateDirectory)/src_texture_image.cpp$(PreprocessSuffix): src/texture/image.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_texture_image.cpp$(PreprocessSuffix)src/texture/image.cpp

$(IntermediateDirectory)/src_tool_mesh.cpp$(ObjectSuffix): src/tool/mesh.cpp $(IntermediateDirectory)/src_tool_mesh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/tool/mesh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_tool_mesh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_tool_mesh.cpp$(DependSuffix): src/tool/mesh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_tool_mesh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_tool_mesh.cpp$(DependSuffix) -MM src/tool/mesh.cpp

$(IntermediateDirectory)/src_tool_mesh.cpp$(PreprocessSuffix): src/tool/mesh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_tool_mesh.cpp$(PreprocessSuffix)src/tool/mesh.cpp

$(IntermediateDirectory)/src_thrid_lodepng_lodepng.cpp$(ObjectSuffix): src/thrid/lodepng/lodepng.cpp $(IntermediateDirectory)/src_thrid_lodepng_lodepng.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/thrid/lodepng/lodepng.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_thrid_lodepng_lodepng.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_thrid_lodepng_lodepng.cpp$(DependSuffix): src/thrid/lodepng/lodepng.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_thrid_lodepng_lodepng.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_thrid_lodepng_lodepng.cpp$(DependSuffix) -MM src/thrid/lodepng/lodepng.cpp

$(IntermediateDirectory)/src_thrid_lodepng_lodepng.cpp$(PreprocessSuffix): src/thrid/lodepng/lodepng.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_thrid_lodepng_lodepng.cpp$(PreprocessSuffix)src/thrid/lodepng/lodepng.cpp

$(IntermediateDirectory)/src_thrid_objloader_Model.cpp$(ObjectSuffix): src/thrid/objloader/Model.cpp $(IntermediateDirectory)/src_thrid_objloader_Model.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/thrid/objloader/Model.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_thrid_objloader_Model.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_thrid_objloader_Model.cpp$(DependSuffix): src/thrid/objloader/Model.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_thrid_objloader_Model.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_thrid_objloader_Model.cpp$(DependSuffix) -MM src/thrid/objloader/Model.cpp

$(IntermediateDirectory)/src_thrid_objloader_Model.cpp$(PreprocessSuffix): src/thrid/objloader/Model.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_thrid_objloader_Model.cpp$(PreprocessSuffix)src/thrid/objloader/Model.cpp

$(IntermediateDirectory)/src_thrid_tinyobjloader_tiny_obj_loader.cc$(ObjectSuffix): src/thrid/tinyobjloader/tiny_obj_loader.cc $(IntermediateDirectory)/src_thrid_tinyobjloader_tiny_obj_loader.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/zhuqian/ClionProjects/RayTracer/src/thrid/tinyobjloader/tiny_obj_loader.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_thrid_tinyobjloader_tiny_obj_loader.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_thrid_tinyobjloader_tiny_obj_loader.cc$(DependSuffix): src/thrid/tinyobjloader/tiny_obj_loader.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_thrid_tinyobjloader_tiny_obj_loader.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/src_thrid_tinyobjloader_tiny_obj_loader.cc$(DependSuffix) -MM src/thrid/tinyobjloader/tiny_obj_loader.cc

$(IntermediateDirectory)/src_thrid_tinyobjloader_tiny_obj_loader.cc$(PreprocessSuffix): src/thrid/tinyobjloader/tiny_obj_loader.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_thrid_tinyobjloader_tiny_obj_loader.cc$(PreprocessSuffix)src/thrid/tinyobjloader/tiny_obj_loader.cc


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


