#Sandbox CMake list

project(Sandbox)

file(
	GLOB sandbox_src
    src/*.hpp
    src/*.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	add_executable(
		Sandbox MACOSX_BUNDLE
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

#copy resource files - TODO: Is there a way to not add a dependancy on this for Clove?
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Clove/res
                ${CMAKE_BINARY_DIR}/Sandbox/res
)

if(MSVC)
	add_custom_command(
	        TARGET ${PROJECT_NAME} POST_BUILD
	        COMMAND ${CMAKE_COMMAND} -E copy_directory
	                ${CMAKE_SOURCE_DIR}/Sandbox/res
	                ${CMAKE_BINARY_DIR}/Sandbox/$<CONFIG>/res
	)
endif()
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Sandbox/res
                ${CMAKE_BINARY_DIR}/Sandbox/res
)
