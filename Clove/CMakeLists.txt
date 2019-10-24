#Clove dependencies
set(EngineFolder "Clove")
set(DependencyFolder "Clove/Dependencies")

set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests")
set(PA_BUILD_SHARED OFF CACHE BOOL "Build shared/dynamic library")
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")

#-dxerr
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_subdirectory(vendor/dxerr)
	set_target_properties(dxerr PROPERTIES FOLDER ${DependencyFolder})
endif()

#-Glad
add_subdirectory(vendor/Glad)
set_target_properties(Glad PROPERTIES FOLDER ${DependencyFolder})

#-glm
add_subdirectory(vendor/glm)
set_target_properties(glm_static PROPERTIES FOLDER ${DependencyFolder}/glm)
set_target_properties(uninstall PROPERTIES FOLDER ${DependencyFolder}/glm)

#-spdlog
add_subdirectory(vendor/spdlog)
set_target_properties(spdlog PROPERTIES FOLDER ${DependencyFolder})

#-stb
add_subdirectory(vendor/stb)
set_target_properties(stb PROPERTIES FOLDER ${DependencyFolder})

#-portaudio
add_subdirectory(vendor/portaudio)
set_target_properties(portaudio_static PROPERTIES FOLDER ${DependencyFolder})

#-libsndfile
add_subdirectory(vendor/libsndfile)
set_target_properties(sndfile PROPERTIES FOLDER ${DependencyFolder})

#-freetype2
add_subdirectory(vendor/freetype2)
set_target_properties(freetype PROPERTIES FOLDER ${DependencyFolder})

#Clove
add_subdirectory(src)
set_target_properties(Clove PROPERTIES FOLDER ${EngineFolder})