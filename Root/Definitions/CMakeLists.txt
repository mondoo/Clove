add_library(
	Root.Definitions INTERFACE
)

target_include_directories(
	Root.Definitions 
	
	INTERFACE 
		include
)

target_compile_definitions(
	Root.Definitions 

	INTERFACE
		#Config
		GARLIC_DEBUG=$<CONFIG:Debug>

		#Platform
		GARLIC_PLATFORM_WINDOWS=$<PLATFORM_ID:Windows>
		GARLIC_PLATFORM_LINUX=$<PLATFORM_ID:Linux>
		GARLIC_PLATFORM_MACOS=$<PLATFORM_ID:Darwin>

		#Flags
		GARLIC_ENABLE_ASSERTS=GARLIC_DEBUG
)