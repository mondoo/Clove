#SPIRV-Cross
set(SPIRV_CROSS_ENABLE_C_API OFF CACHE BOOL "Enable C API wrapper support in static library.")
set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "Enable SPIR-V Cross tests.")
set(SPIRV_CROSS_SKIP_INSTALL ON CACHE BOOL "Skips installation targets.")
add_subdirectory(SPIRV-Cross)

#SPIRV-Headers
set(SPIRV_HEADERS_SKIP_EXAMPLES ON CACHE BOOL "Skip building examples")
set(SPIRV_HEADERS_SKIP_INSTALL ON CACHE BOOL "Skip install")
add_subdirectory(SPIRV-Headers)

#SPIRV-Tools
set(SKIP_SPIRV_TOOLS_INSTALL ON CACHE BOOL "Skip installation")
set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "Skip building the executable and tests along with the library")
set(SPIRV_SKIP_TESTS ON CACHE BOOL "Skip building tests along with the library")
add_subdirectory(SPIRV-Tools)

#glslang
set(SKIP_GLSLANG_INSTALL ON CACHE BOOL "Skip installation")
set(HLSLCC_LIBRARY_SHARED OFF CACHE BOOL "Build shared library instead of static.")
add_subdirectory(glslang)

#shaderc (needs to be last as it depends on above)
set(SHADERC_SKIP_INSTALL ON CACHE BOOL "Skip installation")
set(SHADERC_SKIP_TESTS ON CACHE BOOL "Skip building tests")
add_subdirectory(shaderc)