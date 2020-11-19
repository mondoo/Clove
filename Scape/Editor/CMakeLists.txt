include(CSharpUtilities)

add_executable(
    Scape.Editor
	    App.xaml
        App.xaml.cs

	    AssemblyInfo.cs
        
	    MainWindow.xaml
	    MainWindow.xaml.cs
)

target_link_libraries(
    Scape.Editor

    PUBLIC
        Scape.Membrane   
)

csharp_set_designer_cs_properties(
	AssemblyInfo.cs
)

csharp_set_xaml_cs_properties(
	App.xaml
    App.xaml.cs
    
	MainWindow.xaml
    MainWindow.xaml.cs
)

set_property(SOURCE App.xaml PROPERTY VS_XAML_TYPE "ApplicationDefinition")
set_property(TARGET Scape.Editor PROPERTY VS_DOTNET_FRAMEWORK_VERSION "netcore3.1")

set_property(
    TARGET Scape.Editor PROPERTY VS_DOTNET_REFERENCES
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