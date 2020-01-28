project(Bulb.Desktop LANGUAGES CSharp)

include(CSharpUtilities)

set(
	Files
		App.config
		App.xaml
		App.xaml.cs

		MainWindow.xaml
		MainWindow.xaml.cs
)

add_executable(
	${PROJECT_NAME}
		${Files}
)

csharp_set_xaml_cs_properties(
	${Files}
)

set_property(SOURCE App.xaml PROPERTY VS_XAML_TYPE "ApplicationDefinition")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
#set_property(TARGET ${PROJECT_NAME} PROPERTY WIN64_EXECUTABLE TRUE)

#Add libraries
set_property(
	TARGET ${PROJECT_NAME} PROPERTY VS_DOTNET_REFERENCES
		"Microsoft.CSharp"
		"PresentationCore"
		"PresentationFramework"
		"System"
		"System.Core"
		"System.Data"
		"System.Drawing"
		"System.Windows.Forms"
		"System.Data.DataSetExtensions"
		"System.Net.Http"
		"System.Xaml"
		"System.Xml"
		"System.Xml.Linq"
		"WindowsBase"
)

#Link to wrapper
target_link_libraries(
	${PROJECT_NAME}
		PUBLIC
		PRIVATE
			Bulb.Core
			Bulb.CoreUI
)

#TODO: TEMP - copy over the clove resource files
#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#                ${CMAKE_CURRENT_SOURCE_DIR}/res
#                ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/res
#)
