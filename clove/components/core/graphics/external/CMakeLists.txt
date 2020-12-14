#SPIRV-Headers
add_subdirectory(SPIRV-Headers)

#SPIRV-Cross
set(SPIRV_CROSS_ENABLE_C_API OFF CACHE BOOL "Enable C API wrapper support in static library.")
set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "Enable SPIR-V Cross tests.")
add_subdirectory(SPIRV-Cross)

#SPIRV-Tools
add_subdirectory(SPIRV-Tools)

#glslang
set(HLSLCC_LIBRARY_SHARED OFF CACHE BOOL "Build shared library instead of static.")
add_subdirectory(glslang)

#shaderc
set(SHADERC_SKIP_INSTALL ON CACHE BOOL "Skip installation")
set(SHADERC_SKIP_TESTS ON CACHE BOOL "Skip building tests")
add_subdirectory(shaderc)