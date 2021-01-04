#SPIRV-Cross
set(SPIRV_CROSS_ENABLE_C_API OFF CACHE BOOL "Enable C API wrapper support in static library.")
set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "Enable SPIR-V Cross tests.")
set(SPIRV_CROSS_SKIP_INSTALL ON CACHE BOOL "Skips installation targets.")
add_subdirectory(SPIRV-Cross)