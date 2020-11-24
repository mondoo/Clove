#glslang
set(HLSLCC_LIBRARY_SHARED OFF CACHE BOOL "Build shared library instead of static.")
add_subdirectory(glslang)

#SPIRV-Cross
set(SPIRV_CROSS_ENABLE_C_API OFF CACHE BOOL "Enable C API wrapper support in static library.")
set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "Enable SPIR-V Cross tests.")
add_subdirectory(SPIRV-Cross)