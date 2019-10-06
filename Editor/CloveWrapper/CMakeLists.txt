#Wrapper between the C# editor and C++ engine

project(CloveWrapper)

add_library(
	CloveWrapper SHARED
		Wrapper.hpp
		Wrapper.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_REFERENCES "System")

target_link_libraries(
	${PROJECT_NAME}
		PUBLIC
		PRIVATE
			Clove	
)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/Editor/CloveWrapper/$<CONFIG>/CloveWrapper.dll
				${CMAKE_BINARY_DIR}/Editor/Editor/$<CONFIG>
                #$<TARGET_FILE_DIR:Clove> <-- this will put the file in that projects executable location!
)