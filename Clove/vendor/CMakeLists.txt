set(DependencyFolder "Garlic/Clove/Dependencies")

include(../../cmake/VendorHelpers.cmake)

#dxerr
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_subdirectory(dxerr)
	set_target_properties(dxerr PROPERTIES FOLDER ${DependencyFolder})
endif()

#Glad
add_subdirectory_with_folder(${DependencyFolder} Glad)

#glm
set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests")
add_subdirectory_with_folder(${DependencyFolder} glm)

#spdlog
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
add_subdirectory_with_folder(${DependencyFolder} spdlog)

#stb
add_subdirectory_with_folder(${DependencyFolder} stb)

#glslang
set(HLSLCC_LIBRARY_SHARED OFF CACHE BOOL "Build shared library instead of static.")
add_subdirectory_with_folder(${DependencyFolder}/glslang glslang)

#SPIRV-Cross
set(SPIRV_CROSS_ENABLE_C_API OFF CACHE BOOL "Enable C API wrapper support in static library.")
set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "Enable SPIR-V Cross tests.")
add_subdirectory_with_folder(${DependencyFolder}/SPIRV-Cross SPIRV-Cross)

#openal-soft
set(ALSOFT_DLOPEN OFF CACHE BOOL "Check for the dlopen API for loading optional libs")
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Build example programs")
set(ALSOFT_INSTALL OFF CACHE BOOL "Install main library")
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "Install AmbDec preset files")
set(ALSOFT_INSTALL_CONFIG OFF CACHE BOOL "Install alsoft.conf sample configuration file")
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "Install example programms (alplay, alstream, ...)")
set(ALSOFT_INSTALL_HRTF_DEFS OFF CACHE BOOL "Install HRTF definitions files")
set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "Install utility programs (openal-info, alsoft-config, ...)")
set(ALSOFT_UPDATE_BUILD_VERSION OFF CACHE BOOL "Update git build version info")
set(ALSOFT_UTILS OFF CACHE BOOL "Build utility programs")
add_subdirectory_with_folder(${DependencyFolder}/OpenAL-Soft openal-soft)