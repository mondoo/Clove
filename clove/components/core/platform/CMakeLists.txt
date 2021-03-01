set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/Platform)

set(
	PlatformFiles
		${INCLUDE}/Window.hpp
		${INCLUDE}/Window.inl
)

set(
	WindowsPlatformFiles
		${INCLUDE}/Windows/CloveWindows.hpp
		${SOURCE}/Windows/WindowsMouse.cpp
		${INCLUDE}/Windows/WindowsWindow.hpp
		${SOURCE}/Windows/WindowsWindow.cpp
)

set(
	LinuxPlatformFiles
		${INCLUDE}/Linux/CloveLinux.hpp
		${SOURCE}/Linux/LinuxMouse.cpp
		${INCLUDE}/Linux/LinuxWindow.hpp
		${SOURCE}/Linux/LinuxWindow.cpp
)

set(
	MacOSPlatformFiles
		${INCLUDE}/Mac/CloveMac.hpp
		${SOURCE}/Mac/MacMouse.mm
		${INCLUDE}/Mac/MacWindow.hpp
		${SOURCE}/Mac/MacWindow.mm
)

#Library
add_library(
    ClovePlatform STATIC
        ${INCLUDE}/Input/Keyboard.hpp
		${INCLUDE}/Input/Keyboard.inl
		${SOURCE}/Input/Keyboard.cpp
		${INCLUDE}/Input/KeyCodes.hpp
		${INCLUDE}/Input/Mouse.hpp
		${SOURCE}/Input/Mouse.cpp
        ${INCLUDE}/Input/MouseButtonCodes.hpp
        
		${PlatformFiles}
		${WindowsPlatformFiles}
		${MacOSPlatformFiles}
		${LinuxPlatformFiles}
)

#Includes
target_include_directories(
    ClovePlatform
    
	PUBLIC
		include

)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	find_library(CLV_COCOA_LIB Cocoa)
	find_library(CLV_APPKIT_LIB AppKit)
	find_library(CLV_QUARTZ_LIB QuartzCore)
endif()

#Libraries
target_link_libraries(
    ClovePlatform
     
    PUBLIC
        CloveDefinitions
        CloveDelegate
        CloveLog
        CloveMaths

    #Linux
	PRIVATE
		$<$<PLATFORM_ID:Linux>:X11>
		$<$<PLATFORM_ID:Linux>:dl>

	#MacOS
	PRIVATE
		$<$<PLATFORM_ID:Darwin>:${CLV_COCOA_LIB}>
		$<$<PLATFORM_ID:Darwin>:${CLV_APPKIT_LIB}>
		$<$<PLATFORM_ID:Darwin>:${CLV_QUARTZ_LIB}>
)

#Don't compile platform specific files
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set_source_files_properties(${LinuxPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_source_files_properties(${WindowsPlatformFiles} ${LinuxPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set_source_files_properties(${WindowsPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()
