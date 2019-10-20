#Sandbox CMake list

project(Sandbox)

file(
	GLOB sandbox_src
    src/*.hpp
    src/*.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	add_executable(
		Sandbox
		MACOSX_BUNDLE
		${sandbox_src}
	)
else()
	add_executable(
		Sandbox
		${sandbox_src}
	)
endif()

target_include_directories(
	Sandbox
	PUBLIC
		src
)

target_link_libraries(Sandbox PRIVATE Clove)

#Platform specifics
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set_target_properties(Sandbox PROPERTIES 
		MACOSX_BUNDLE_BUNDLE_NAME "Sandbox"
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Sandbox/CMake/Info.plist.in
	)
endif()