#Editor CMake list

project(Bulb.Core LANGUAGES CSharp)

include(CSharpUtilities)

#Add all c# files
add_executable(
	${PROJECT_NAME}
	Temp.cs
	#TODO
)

#Set other csproj file flags
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
