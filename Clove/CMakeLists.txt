set(EngineFolder "Garlic/Clove")

#Dependencies
add_subdirectory(vendor)

#Clove
add_subdirectory(src)
set_target_properties(Clove PROPERTIES FOLDER ${EngineFolder})

#Includes
target_include_directories(
	Clove

	#Clove
	PUBLIC
		include

	#Libraries
	INTERFACE
        vendor/glm

	PRIVATE
		$<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/vendor/wglext>
)

#Libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	find_library(CLV_COCOA_LIB Cocoa)
	find_library(CLV_METAL_LIB Metal)
	find_library(CLV_APPKIT_LIB AppKit)
	find_library(CLV_QUARTZ_LIB QuartzCore)
endif()

target_link_libraries(
	Clove

    #Cross platform
    PUBLIC
		spdlog
		$<$<BOOL:${GAR_CLV_IMGUI_COMPATIBLE}>:Glad>

		#Below are public until wrappers have been made
		portaudio_static

	PRIVATE 
		stb
		$<$<NOT:$<BOOL:${GAR_CLV_IMGUI_COMPATIBLE}>>:Glad>
		glm
		glslang
		glslang-default-resource-limits
		SPIRV
		spirv-cross-core
		spirv-cross-glsl
		spirv-cross-hlsl
		spirv-cross-msl

	#Windows
	PRIVATE
		$<$<PLATFORM_ID:Windows>:dxerr>
		$<$<PLATFORM_ID:Windows>:opengl32>
		$<$<PLATFORM_ID:Windows>:d3d11>
		$<$<PLATFORM_ID:Windows>:dxguid>
		$<$<PLATFORM_ID:Windows>:d3dcompiler>

	#Linux
	PRIVATE
		$<$<PLATFORM_ID:Linux>:GLX>
		$<$<PLATFORM_ID:Linux>:X11>
		$<$<PLATFORM_ID:Linux>:dl>

	#MacOS
	PRIVATE
		$<$<PLATFORM_ID:Darwin>:${CLV_COCOA_LIB}>
		$<$<PLATFORM_ID:Darwin>:${CLV_METAL_LIB}>
		$<$<PLATFORM_ID:Darwin>:${CLV_APPKIT_LIB}>
		$<$<PLATFORM_ID:Darwin>:${CLV_QUARTZ_LIB}>
)

#Precompiled header
target_precompile_headers(
	Clove

	#Generic headers
	PUBLIC
		<iostream>
		<memory>
		<utility>
		<algorithm>
		<functional>
		<sstream>
		<optional>

	#Data structures
	PUBLIC
        <string>
        <array>
		<vector>
		<map>
		<unordered_map>
		<set>
		<unordered_set>

	#Clove headers
	PUBLIC
		[["Clove/Core/Core.hpp"]]
		[["Clove/Core/Log.hpp"]]
		[["Clove/Core/Maths/MathsTypes.hpp"]]
		[["Clove/Core/Maths/Maths.hpp"]]
		[["Clove/Core/Maths/MathsHelpers.hpp"]]
		[["Clove/Core/Utils/Delegate.hpp"]]
		[["Clove/Core/Utils/Timer.hpp"]]
)

#Preprocessor
target_compile_definitions(
	Clove

	#General
	PUBLIC
		CLV_ENABLE_PROFILING=$<BOOL:${GAR_CLV_ENABLE_PROFILING}>
		CLV_ENABLE_MEMORY_DEBUGGING=$<BOOL:${GAR_CLV_ENABLE_MEMORY_DEBUGGING}>
		CLV_TRACK_MEMORY_ALLOCATIONS=$<BOOL:${GAR_TRACK_MEMORY_ALLOCATIONS}>

	#Config
	PUBLIC
		CLV_DEBUG=$<CONFIG:Debug>
		CLV_RELEASE=$<CONFIG:Release>

	#Platform
	PUBLIC
		CLV_PLATFORM_WINDOWS=$<PLATFORM_ID:Windows>
		CLV_PLATFORM_LINUX=$<PLATFORM_ID:Linux>
		CLV_PLATFORM_MACOS=$<PLATFORM_ID:Darwin>

	#Engine
	INTERFACE
		CLV_ENGINE=0
	PRIVATE
		CLV_ENGINE=1

	#Libraries
	PUBLIC
		GLM_FORCE_DEPTH_ZERO_TO_ONE
)

#Examples
if(GAR_BUILD_EXAMPLES)
	add_subdirectory(Examples)
endif()