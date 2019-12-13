#Wrapper between the C# editor and C++ engine

project(Bulb.Core)

set(
	Files
		EditorLayer.hpp
		EditorLayer.cpp
		EditorSession.hpp
		EditorSession.cpp
		EditorWindowProxy.hpp
		EditorWindowProxy.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Files})

add_library(
	${PROJECT_NAME} SHARED
		${Files}
)

target_link_libraries(
	${PROJECT_NAME}
		PUBLIC
		PRIVATE
			Clove	
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_REFERENCES "System")