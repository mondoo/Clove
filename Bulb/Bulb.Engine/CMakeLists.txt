#Wrapper between the C# editor and C++ engine

project(Bulb.Engine)

add_library(
	${PROJECT_NAME} SHARED
		Wrapper.hpp
		Wrapper.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_REFERENCES "System")

#Disable PCHs for the CLR project to avoid issues with compilation
set_target_properties(${PROJECT_NAME} PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)

target_link_libraries(
	${PROJECT_NAME}
		PUBLIC
		PRIVATE
			Clove	
)

#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_BINARY_DIR}/Bulb/Bulb.Engine/$<CONFIG>/Bulb.Engine.dll
#				${CMAKE_BINARY_DIR}/Bulb/Bulb.Desktop.Windows/$<CONFIG>
#                #$<TARGET_FILE_DIR:Clove> <-- this will put the file in that projects executable location!
#)