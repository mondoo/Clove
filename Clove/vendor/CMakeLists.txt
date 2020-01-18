set(DependencyFolder "Garlic/Clove/Dependencies")

#Vendor defaults
set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests")
set(PA_BUILD_SHARED OFF CACHE BOOL "Build shared/dynamic library")
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")

#dxerr
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_subdirectory(dxerr)
	set_target_properties(dxerr PROPERTIES FOLDER ${DependencyFolder})
endif()

#Glad
add_subdirectory(Glad)
set_target_properties(Glad PROPERTIES FOLDER ${DependencyFolder})

#glm
add_subdirectory(glm)
set_target_properties(glm_static PROPERTIES FOLDER ${DependencyFolder})

#spdlog
add_subdirectory(spdlog)
set_target_properties(spdlog PROPERTIES FOLDER ${DependencyFolder})

#stb
add_subdirectory(stb)
set_target_properties(stb PROPERTIES FOLDER ${DependencyFolder})

#portaudio
add_subdirectory(portaudio)
set_target_properties(portaudio_static PROPERTIES FOLDER ${DependencyFolder}/portaudio)
set_target_properties(uninstall PROPERTIES FOLDER ${DependencyFolder}/portaudio)

#libsndfile
add_subdirectory(libsndfile)
set_target_properties(sndfile PROPERTIES FOLDER ${DependencyFolder})