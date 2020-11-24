add_subdirectory(components)

#Library
add_library(
	Clove STATIC
		source/temp.cpp	
)

#Includes
target_include_directories(
	Clove

	#Clove
	PUBLIC
		include
)

target_link_libraries(
	Clove

	#Components
	PUBLIC
		#Core
		CloveAudio
		CloveGraphics
		ClovePlatform

		#Systems
		CloveECS
		
		#Utilities
		CloveCast
		CloveDefinitions
		CloveDelegate
		CloveDeltaTime
		CloveEvent
		CloveExpected
		CloveLog
		CloveMaths
		CloveMemory
		CloveTimer		
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
)

#Preprocessor Clove
target_compile_definitions(
	Clove

	#General
	PUBLIC
		CLV_ENABLE_MEMORY_DEBUGGING=$<BOOL:${GAR_CLV_ENABLE_MEMORY_DEBUGGING}>
		CLV_IMGUI_COMPATIBLE=$<BOOL:${GAR_CLV_IMGUI_COMPATIBLE}>

	#Libraries
	PUBLIC
		GLM_FORCE_RADIANS
		GLM_FORCE_LEFT_HANDED
		GLM_FORCE_DEPTH_ZERO_TO_ONE
)