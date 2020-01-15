set(DependencyFolder "Garlic/Clove/Dependencies")

function(get_all_targets _result _dir)
    get_property(_subdirs DIRECTORY "${_dir}" PROPERTY SUBDIRECTORIES)
    foreach(_subdir IN LISTS _subdirs)
        get_all_targets(${_result} "${_subdir}")
    endforeach()
    get_property(_sub_targets DIRECTORY "${_dir}" PROPERTY BUILDSYSTEM_TARGETS)
    set(${_result} ${${_result}} ${_sub_targets} PARENT_SCOPE)
endfunction()

function(add_subdirectory_with_folder _folder_name _folder)
    add_subdirectory(${_folder} ${ARGN})
    get_all_targets(_targets "${_folder}")
	foreach(_target IN LISTS _targets)
		get_target_property(target_type ${_target} TYPE)
		if(NOT ${target_type} STREQUAL "INTERFACE_LIBRARY")
			set_target_properties(
				${_target}
				PROPERTIES FOLDER "${_folder_name}"
			)
		endif()
    endforeach()
endfunction()

#Vendor defaults
set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests")
set(PA_BUILD_SHARED OFF CACHE BOOL "Build shared/dynamic library")
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Set when you want to build the Bullet 2 demos")
set(BUILD_CLSOCKET OFF CACHE BOOL "Set when you want to build apps with enet TCP networking support")
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Set to build original Bullet CPU demos")
set(BUILD_EGL OFF CACHE BOOL "OpenGL/EGL")
set(BUILD_ENET OFF CACHE BOOL "Set when you want to build apps with enet UDP networking support")
set(BUILD_EXTRAS OFF CACHE BOOL "Set when you want to build extras")
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "Set when you want to build Bullet 3 OpenGL3+ demos")
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Build unit tests")
set(USE_GLUT OFF CACHE BOOL "Use GLUT")
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "Use graphical benchmark")
set(HLSLCC_LIBRARY_SHARED OFF CACHE BOOL "Build shared library instead of static.")

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

#freetype2
add_subdirectory(freetype2)
set_target_properties(freetype PROPERTIES FOLDER ${DependencyFolder})

#bullet3
add_subdirectory_with_folder(${DependencyFolder}/bullet3 bullet3)

#glslang
add_subdirectory_with_folder(${DependencyFolder}/glslang glslang)

#SPIRV-Cross
add_subdirectory_with_folder(${DependencyFolder}/SPIRV-Cross SPIRV-Cross)
