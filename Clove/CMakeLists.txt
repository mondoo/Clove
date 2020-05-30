set(EngineFolder "Garlic/Clove")

#Dependencies
add_subdirectory(vendor)

#Clove
add_subdirectory(src)
set_target_properties(Clove PROPERTIES FOLDER ${EngineFolder})

find_package(Vulkan REQUIRED)

#Includes
target_include_directories(
	Clove

	#Clove
	PUBLIC
		include

	#Libraries
	PUBLIC
		Vulkan::Vulkan #Temp public
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

		Vulkan::Vulkan #Temp public

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
		$<$<PLATFORM_ID:Windows>:Vulkan::Vulkan>

	#Linux
	PRIVATE
		$<$<PLATFORM_ID:Linux>:GLX>
		$<$<PLATFORM_ID:Linux>:X11>
		$<$<PLATFORM_ID:Linux>:dl>
		$<$<PLATFORM_ID:Linux>:Vulkan::Vulkan>

	#MacOS
	PRIVATE
		$<$<PLATFORM_ID:Darwin>:${CLV_COCOA_LIB}>
		$<$<PLATFORM_ID:Darwin>:${CLV_METAL_LIB}>
		$<$<PLATFORM_ID:Darwin>:${CLV_APPKIT_LIB}>
		$<$<PLATFORM_ID:Darwin>:${CLV_QUARTZ_LIB}>
)

#Precompiled header ($<ANGLE-R> makes sure #include ">" doesn't happen)
target_precompile_headers(
	Clove

	#Generic headers
	PUBLIC
		$<$<COMPILE_LANGUAGE:CXX>:<iostream$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<memory$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<utility$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<algorithm$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<functional$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<sstream$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<optional$<ANGLE-R>>

	#Data structures
	PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:<string$<ANGLE-R>>
        $<$<COMPILE_LANGUAGE:CXX>:<array$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<vector$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<map$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<unordered_map$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<set$<ANGLE-R>>
		$<$<COMPILE_LANGUAGE:CXX>:<unordered_set$<ANGLE-R>>

	#Clove headers
	PUBLIC
		$<$<COMPILE_LANGUAGE:CXX>:"Clove/Core.hpp">
		$<$<COMPILE_LANGUAGE:CXX>:"Clove/Log.hpp">
		$<$<COMPILE_LANGUAGE:CXX>:"Clove/Maths/MathsTypes.hpp">
		$<$<COMPILE_LANGUAGE:CXX>:"Clove/Maths/Maths.hpp">
		$<$<COMPILE_LANGUAGE:CXX>:"Clove/Maths/MathsHelpers.hpp">
		$<$<COMPILE_LANGUAGE:CXX>:"Clove/Utils/Delegate.hpp">
		$<$<COMPILE_LANGUAGE:CXX>:"Clove/Utils/DeltaTime.hpp">
		$<$<COMPILE_LANGUAGE:CXX>:"Clove/Utils/Timer.hpp">
)

#Preprocessor Clove
target_compile_definitions(
	Clove

	#General
	PUBLIC
		CLV_ENABLE_PROFILING=$<BOOL:${GAR_CLV_ENABLE_PROFILING}>
		CLV_ENABLE_MEMORY_DEBUGGING=$<BOOL:${GAR_CLV_ENABLE_MEMORY_DEBUGGING}>
		CLV_TRACK_MEMORY_ALLOCATIONS=$<BOOL:${GAR_TRACK_MEMORY_ALLOCATIONS}>
		CLV_IMGUI_COMPATIBLE=$<BOOL:${GAR_CLV_IMGUI_COMPATIBLE}>

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
		GLM_FORCE_RADIANS
		GLM_FORCE_LEFT_HANDED
		GLM_FORCE_DEPTH_ZERO_TO_ONE
)

#Examples
if(GAR_BUILD_EXAMPLES)
	add_subdirectory(Examples)
endif()