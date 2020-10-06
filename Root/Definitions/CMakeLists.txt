#CMake
option(GAR_ENABLE_DEBUG_DEFINITION "Sets the GARLIC_DEBUG macro to 1. Allowing for various debug functionality / information to be enabled" OFF)

#Library
add_library(
	Root.Definitions INTERFACE
)

#Include
target_include_directories(
	Root.Definitions 
	
	INTERFACE 
		include
)

#Preprocessor
target_compile_definitions(
	Root.Definitions 

	INTERFACE
		#Platform
		GARLIC_PLATFORM_WINDOWS=$<PLATFORM_ID:Windows>
		GARLIC_PLATFORM_LINUX=$<PLATFORM_ID:Linux>
		GARLIC_PLATFORM_MACOS=$<PLATFORM_ID:Darwin>

		#Flags
		GARLIC_DEBUG=$<BOOL:${GAR_ENABLE_DEBUG_DEFINITION}>
		GARLIC_ENABLE_ASSERTIONS=$<BOOL:${GAR_ENABLE_ASSERTIONS}>
)