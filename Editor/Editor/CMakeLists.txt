#Editor CMake list

project(CloveEditor LANGUAGES CSharp)

include(CSharpUtilities)

#Add all c# files
add_executable(
	CloveEditor
		App.config
		App.xaml
		App.xaml.cs
		MainWindow.xaml
		MainWindow.xaml.cs

		Properties/AssemblyInfo.cs
		Properties/Resources.resx
		Properties/Resources.Designer.cs
		Properties/Settings.settings
		Properties/Settings.Designer.cs
)

#Link designer files, xaml files, and other properties files with their corresponding cs files
csharp_set_designer_cs_properties(
    Properties/AssemblyInfo.cs

    Properties/Resources.resx
    Properties/Resources.Designer.cs

    Properties/Settings.settings
    Properties/Settings.Designer.cs
)

csharp_set_xaml_cs_properties(
    App.xaml
    App.xaml.cs

    MainWindow.xaml
    MainWindow.xaml.cs
)

#Set app App.xaml properties file as program entry point (if project is a WPF project)
set_property(SOURCE App.xaml PROPERTY VS_XAML_TYPE "ApplicationDefinition")

#Set other csproj file flags
set_property(TARGET CloveEditor PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
set_property(TARGET CloveEditor PROPERTY WIN32_EXECUTABLE TRUE)

#Add libraries
set_property(
	TARGET CloveEditor PROPERTY VS_DOTNET_REFERENCES
		"Microsoft.CSharp"
		"PresentationCore"
		"PresentationFramework"
		"System"
		"System.Core"
		"System.Data"
		"System.Data.DataSetExtensions"
		"System.Net.Http"
		"System.Xaml"
		"System.Xml"
		"System.Xml.Linq"
		"WindowsBase"
)

#Link to wrapper
target_link_libraries(
	CloveEditor
		PUBLIC
		PRIVATE
			CloveWrapper
)