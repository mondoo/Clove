include(CSharpUtilities)

add_executable(
    Scape.Editor
	    EditorApp.xaml
        EditorApp.xaml.cs

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
	EditorApp.xaml
    EditorApp.xaml.cs
    
	MainWindow.xaml
    MainWindow.xaml.cs
)

set_source_files_properties(EditorApp.xaml PROPERTIES VS_XAML_TYPE "ApplicationDefinition")

set_target_properties(Scape.Editor 
	PROPERTIES 
		VS_DOTNET_FRAMEWORK_VERSION "4.8"
        VS_DOTNET_REFERENCES "Microsoft.CSharp;PresentationCore;PresentationFramework;System;System.Core;System.Data;System.Xaml;System.Xml;WindowsBase"
)

#Set the output directory of OpenAL
set_target_properties(OpenAL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})