#Editor CMake list

project(Bulb.Engine LANGUAGES CSharp)

include(CSharpUtilities)

#Add all c# files
add_library(
	${PROJECT_NAME} SHARED
	EngineSession.cs
	#TODO
)

#Set other csproj file flags
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")

target_link_libraries(
	${PROJECT_NAME}
		PUBLIC #public until all wrappers have been made
			Bulb.Engine.Clove
)