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
	PUBLIC
		vendor/OBJ-Loader/Source
	INTERFACE
        vendor/glm
		#TODO: Get rid of the lbsndfile paths below (see Sound.hpp)
		vendor/libsndfile/src
		${CMAKE_CURRENT_BINARY_DIR}/vendor/libsndfile/src
	PRIVATE
        vendor/OBJ-Loader/Source
		$<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/vendor/wglext>
)

#Libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	find_library(CLV_COCOA_LIB Cocoa)
	find_library(CLV_METAL_LIB Metal)
	find_library(CLV_METALKIT_LIB MetalKit)
endif()

target_link_libraries(
	Clove
    #Cross platform
    PUBLIC
		spdlog
		
		#Below are public until wrappers have been made
		portaudio_static
	PRIVATE 
		stb
		Glad
		glm
		sndfile
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
		$<$<PLATFORM_ID:Darwin>:${CLV_METALKIT_LIB}>
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
	#Config
	PUBLIC
		CLV_DEBUG=$<CONFIG:Debug>
		CLV_RELEASE=$<CONFIG:Release>
	#Platform
	PUBLIC
		CLV_PLATFORM_WINDOWS=$<PLATFORM_ID:Windows>
		CLV_PLATFORM_LINUX=$<PLATFORM_ID:Linux>
		CLV_PLATFORM_MACOS=$<PLATFORM_ID:Darwin>
	PRIVATE
		$<$<PLATFORM_ID:Darwin>:GL_SILENCE_DEPRECATION> #Silences the MacOS gl deprecation messages
	#Engine
	INTERFACE
		CLV_ENGINE=0
	PRIVATE
		CLV_ENGINE=1
)

#Examples
if(GAR_BUILD_EXAMPLES)
	add_subdirectory(Examples)
endif()